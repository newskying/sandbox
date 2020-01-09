package main

/*test with curl
1 url /rpctest                       -- http server Handler
2 "method":"HelloS.Say" in json body -- RegisterService

curl -X POST -H "Content-Type: application/json" \
-d '{"method":"HelloS.Say","params":[{"Who":"Test"}], "id":"1"}' \
http://localhost:10000/rpctest
*/

import (
	"bytes"
	j "encoding/json"
	"flag"
	"fmt"
	"github.com/gorilla/rpc"
	"github.com/gorilla/rpc/json"
	"io/ioutil"
	"log"
	"math/rand"
	"net/http"
	"strings"
	"time"
)

type HelloArgs struct {
	Who string
}

type HelloReply struct {
	Message string
}

type HelloResponse struct {
	Result HelloReply `json:"result"`
	Error  string     `json:"error"`
	Id     string     `json:"id"`
}

type HelloRequest struct {
	Method string      `json:"method"`
	Params []HelloArgs `json:"params"`
	Id     string      `json:"id"`
}

func init() {
	rand.Seed(time.Now().UnixNano())
}

func (h *HelloRequest) Say_Request(p HelloArgs) *HelloRequest {
	h.Method = "HelloS.Say" //HelloService.Say
	h.Params = []HelloArgs{p}
	h.Id = fmt.Sprintf("%v.%v", time.Now().UnixNano(), rand.Int63())
	return h
}

type HelloService struct{}

func (h *HelloService) Say(r *http.Request, args *HelloArgs, reply *HelloReply) error {
	reply.Message = "Hello, " + args.Who + "!"
	log.Printf("request: %v\nargs: %v\nreply: %v", r, args, reply)
	time.Sleep(3 * time.Second)
	return nil
}

func main() {
	flag.Parse()
	if len(flag.Args()) > 0 { //http client
		log.Printf("Calling RPC Server on :10000\n")
		r := &HelloRequest{}
		if b, err := j.Marshal(r.Say_Request(HelloArgs{flag.Arg(0)})); err != nil {
			log.Fatal(err)
		} else {
			log.Printf("json %s", b)
			if res, err := http.Post("http://localhost:10000/rpctest", "application/json", strings.NewReader(string(b))); err != nil {
				log.Fatal(err)
			} else {
				log.Printf("res : %v", res)
				hello, err := ioutil.ReadAll(res.Body)
				res.Body.Close()
				if err != nil {
					log.Fatal(err)
				}
				log.Printf("raw %s", hello)
				var data HelloResponse
				if err := j.Unmarshal(hello, &data); err != nil {
					log.Fatal(err)
				}
				log.Printf("parsed %+v", data)
			}
		}

	} else { //http server
		log.Printf("Starting RPC Server on :10000\n")
		s := rpc.NewServer()
		s.RegisterCodec(json.NewCodec(), "application/json")
		s.RegisterService(new(HelloService), "HelloS") //s.RegisterService(new(HelloService), "")  will register an method:"HelloService.Say"
		s.RegisterInterceptFunc(func(i *rpc.RequestInfo) *http.Request {
			req := new(HelloRequest)
			err := j.NewDecoder(i.Request.Body).Decode(req)
			req.Id = ""
			b, err := j.Marshal(req)
			if err != nil {
				log.Fatal(err)
			}
			br := bytes.NewReader(b)
			newReq, err := http.NewRequest(i.Request.Method, i.Request.URL.String(), br)
			if err != nil {
				log.Fatal(err)
			}
			return newReq
		})
		http.Handle("/rpctest", s)
		http.ListenAndServe("localhost:10000", nil)
	}
}
