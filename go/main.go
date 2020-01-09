package main

import (
	"fmt"
	//"github.com/fiorix/go-smpp/smpp/pdu" //in GOPATH
	"bufio"
	"container/list"
	"context"
	"flag"
	"io"
	"math"
	"math/rand"
	"net"
	"os"
	"os/signal"
	"reflect"
	"runtime"
	"strings"
	"sync"
	"syscall"
	"time"
)

//test_chan
func xrange(start int) chan int {
	var ch chan int = make(chan int)
	go func() {
		for i := start; ; i++ {
			ch <- i
		}
	}()
	return ch
}

func test_chan() {
	generator := xrange(0)
	for i := 0; i < 5; i++ {
		fmt.Print(<-generator)
	}
}

//test_close_chan
func test_close_chan() {
	quit := make(chan int)
	go func(chan int) {
		for {
			select {
			case <-quit:
				println("...quit")
				return
			default:
				println("doing something...")
				time.Sleep(1 * time.Second)
			}
		}
	}(quit)
	//if true {   // {} if{} is NOT a lifespan context, should use lambda function
	func() {
		defer close(quit)
		time.Sleep(3 * time.Second)
	}()
	time.Sleep(3 * time.Second)
}

//test_coroutine
var goroutine_len = 3
var quit []chan int = make([]chan int, goroutine_len)

func loop(index int, ch chan int) {
	for i := 0; i < 5; i++ {
		//runtime.Gosched() // explicit yield cpu time to other coroutine
		fmt.Printf("in loop[%v] %d\n", index, i)
	}
	fmt.Printf("to quit %v\n", index)
	ch <- 1
}

func say(s string) {
	for i := 0; i < 5; i++ {
		fmt.Println(s)
	}
}

func test_coroutine() {
	fmt.Printf("current NumCPU:%d\n", runtime.NumCPU())
	runtime.GOMAXPROCS(8) //max 2 cpu core
	for i := 0; i < goroutine_len; i++ {
		fmt.Printf("start goroutine %v\n", i)
		quit[i] = make(chan int) // IMPORTANT!!! must init chan, NOT only []chan
		go loop(i, quit[i])
	}
	for i := 0; i < goroutine_len; i++ {
		fmt.Printf("...wait goroutine %v stop\n", i)
		<-quit[i]
		fmt.Printf("goroutine %v quit!!!\n", i)
	}
	//for i, ch := range quit {
	//fmt.Printf("...wait goroutine %v stop\n", i)
	//<-ch
	//fmt.Printf("goroutine %v quit!!!\n", i)
	//}
	//go say("world")
	//for {
	//}
}

type HandleFun func(int) int

type S struct {
	a    int
	done sync.WaitGroup
	fun  HandleFun
}

func test_coroutine_sig_quit_and_wait() {
	finish := make(chan struct{})
	var s = S{}
	fmt.Printf("S:%+v", s) // default sync.WaitGroup could be use. default HandleFun is nil
	//var done sync.WaitGroup // = sync.WaitGroup{}
	for i := 0; i < goroutine_len; i++ {
		fmt.Println("start goroutine ", i)
		s.done.Add(1)
		go func(i int) {
			select {
			case <-time.After(1 * time.Minute):
			case <-finish:
			}
			fmt.Printf("goroutine %v quit\n", i)
			s.done.Done()
		}(i)
	}
	t0 := time.Now()
	close(finish)
	s.done.Wait()
	fmt.Printf("Waited %v for goroutine to stop\n", time.Since(t0))
}

func WaitMany(a, b chan bool) {
	for a != nil || b != nil {
		select {
		case <-a:
			a = nil
		case <-b:
			b = nil
		}
	}
}

func test_coroutine_waitmany() {
	a, b := make(chan bool), make(chan bool)
	t0 := time.Now()
	go func() {
		fmt.Println("signal a")
		close(a)
		fmt.Println("signal b")
		close(b)
	}()
	fmt.Println("waited for signal a and signal b")
	WaitMany(a, b)
	fmt.Printf("waited %v for WaitMany\n", time.Since(t0))
}

//test_OO: polymorphic and dynamic heritage
type CarIf interface {
	Set(name string, age int)
}

type Car struct {
	Name string
	Age  int
}

func (c *Car) Set(name string, age int) {
	c.Name = name
	c.Age = age
}

type Car2 struct {
	Name string
}

type Train struct {
	Car
	Car2
	venderName string
	int
}

/*
func (t *Train) Set(age int) {
	t.int = age
}
*/
func test_OO() {
	var train Train
	//train.Set(15)
	fmt.Println(train.int)
	train.int = 20
	fmt.Println(train.int)
	train.Car.Set("oo", 30)
	var pCar CarIf = &train
	pCar.Set("bar", 40)
}

func test_signal() {
	c := make(chan os.Signal)
	signal.Notify(c)
	fmt.Println("start signal")
	//s := <-c
	//fmt.Println("exit signal:", s)
	go func() {
		for s := range c {
			switch s {
			case syscall.SIGHUP, syscall.SIGINT, syscall.SIGTERM, syscall.SIGQUIT:
				fmt.Println("exit signal:", s)
				os.Exit(0)
			default:
				fmt.Println("other signal")
				os.Exit(0)
			}
		}
	}()
	sum := 0
	for {
		sum++
		fmt.Println("sum:", sum)
		time.Sleep(time.Second)
	}
}

func do_stuff(x int) int {
	time.Sleep(time.Duration(rand.Intn(10)) * time.Millisecond)
	return 100 - x
}

func branch(x int) chan int {
	ch := make(chan int)
	go func() {
		ch <- do_stuff(x)
	}()
	return ch
}

func fanln(chs ...chan int) chan int {
	ch := make(chan int)
	for _, c := range chs {
		//NOTICE, must by value
		go func(c chan int) { ch <- <-c }(c) //join
	}
	return ch
}

func fanln_select(chs ...chan int) chan int {
	ch := make(chan int)
	go func() {
		for i := 0; i < len(chs); i++ {
			select {
			case v := <-chs[i]:
				ch <- v
			}
		}
	}()
	return ch
}

func test_fan() {
	//result := fanln(branch(1), branch(2), branch(3))
	result := fanln_select(branch(1), branch(2), branch(3))
	for i := 0; i < 3; i++ {
		fmt.Println(<-result)
	}
}

func test_select() {
	c1, c2, c3 := branch(1), branch(2), branch(3)
	ch := make(chan int)

	go func() { //open a goroutine to monitor c1,c2,c3 and gather data into ch
		for {
			select { //monitor c1,c2,c3 and gather into ch
			case v1 := <-c1:
				ch <- v1
			case v2 := <-c2:
				ch <- v2
			case v3 := <-c3:
				ch <- v3
			}
		}
	}()

	//block main thread, fetch channel ch
	for i := 0; i < 3; i++ {
		fmt.Println(<-ch)
	}
}

func test_timeout() {
	timeout := time.After(1 * time.Second)
	select {
	case <-timeout:
		fmt.Println("timeout")
	}
}

//sieve method to output prime number
func filter(in chan int, number int) chan int {
	//input a int queue: chan(in), filter number's multiple, and put others into chan(out)
	out := make(chan int)
	go func() {
		for {
			i := <-in
			if i%number != 0 {
				out <- i
			}
		}
	}()
	return out
}

//daisy-chain
func test_daisy_chain(max int) {
	nums := xrange(2) //generator should not start from 0, or main will panic by divide 0
	number := <-nums  //init an interger generator

	for number < max { //number as sieve
		fmt.Printf("%d ", number)   //output prime number, sieve is the first prime number
		nums = filter(nums, number) //seive number's multiples
		number = <-nums             //update sieve
	}
}

func test_sieve() {
	var max int = 1000
	size := math.Sqrt(float64(max))
	var sieve []int = make([]int, 0, int(size))
	fmt.Println("size:", len(sieve))
Next:
	for i := 2; i < max; i++ {
		j := 0
		for ; j < len(sieve); j++ {
			if i%sieve[j] == 0 {
				continue Next
			}
		}
		sieve = append(sieve, i)
	}
	fmt.Println(sieve)
}

func test_context_cancel() {
	ctx, cancel := context.WithCancel(context.Background())
	//defer cancel()

	//flags := make([]chan int, 4)
	for i := 1; i < 3; i++ {
		//goroutine
		go func(id int) {
			//inner goroutine
			go func(id int) {
				//for range time.Tick(time.Second) {
				for {
					select {
					case <-ctx.Done():
						println(id, " subtask quit")
						//quit <- 0
						return
					default:
						time.Sleep(1 * time.Second)
						fmt.Println(id, "monitor subtask working")
					}
				}
			}(id + 50)
			for {
				select {
				case <-ctx.Done():
					println(id, " quit")
					//quit <- 0
					return
				default:
					fmt.Println(id, "monitor working")
					time.Sleep(1 * time.Second)
				}
			}
		}(i)
	}
	time.Sleep(3 * time.Second)
	println("to cancel")
	cancel()
	println("finish cancel")
	//for {
	//for f := range flags {
	//select {
	////case <-f:
	//}

	//}
	//}
	time.Sleep(3 * time.Second)
	println("main gone")
}

func test_slice() {
	fmt.Println("--------- numbers:slice numbers2:array numbers3:slice of numbers2")
	var numbers []int
	numbers = append(numbers, 1, 2, 3, 4)
	var numbers2 = [5]int{1, 2, 3, 4, 5}
	//numbers2 = [5]int{1, 2, 3, 4, 5}
	numbers3 := numbers2[0:len(numbers2)] //number3 is a slice(reference) of array numbers2
	fmt.Println(numbers)
	fmt.Println(numbers2)
	fmt.Println(numbers3)

	fmt.Println("---------change numbers, also change numbers3 and numbers2")
	numbers = numbers3
	numbers[0] = 8 // change numbers, also change numbers3 and numbers2
	fmt.Println(numbers)
	fmt.Println(numbers2)
	fmt.Println(numbers3)

	fmt.Println("---------copy will NOT generate a new slice")
	copy(numbers3, numbers) //copy will NOT generate a new slice
	numbers[0] = 3
	fmt.Println(numbers)
	fmt.Println(numbers2)
	fmt.Println(numbers3)

	fmt.Println("---------append() makes \"numbers\" change to a new slice")
	numbers = append(numbers, 6, 7) //numbers change to a new slice
	fmt.Println(numbers)
	fmt.Println(numbers2)
	fmt.Println(numbers3)

	fmt.Println("---------append() makes \"numbers\" change to a new slice")
	numbers[0] = 9
	fmt.Println(numbers)
	fmt.Println(numbers2)
	fmt.Println(numbers3)
}

func f() {
	defer func() {
		if r := recover(); r != nil {
			fmt.Println("Recovered in f", r)
		}
	}()
	fmt.Println("Calling g.")
	g(0)
	fmt.Println("Returned normally from g.")
}

func g(i int) {
	if i > 3 {
		fmt.Println("Panicking!")
		panic(fmt.Sprintf("%v", i))
	}
	defer fmt.Println("Defer in g", i)
	fmt.Println("Printing in g", i)
	g(i + 1)
}

func test_panic_defer() {
	f()
	fmt.Println("Return normally from f.")
}

func test_net() {
	l, err := net.Listen("tcp", "0.0.0.0:8888")
	if err != nil {
		println("error in listen on 8888")
	}
	ch := make(chan int)
	go func() {
		println("to listen on 0.0.0.0:8888...")
		_, err := l.Accept()
		if err != nil {
			println("accept error, return", err)
			ch <- 2
			return
		}
		println("to quit...")
		ch <- 1
	}()
	println("wait listen...")
	time.Sleep(2 * time.Second)
	l.Close() //Listener.Close() could break Accept block
	<-ch
	println("exit")
}

func test_Copy() {
	io.Copy(os.Stdout, os.Stdin)
	fmt.Println("Got EOF -- bye")
}

func test_bufio_readslice() {
	reader := bufio.NewReader(strings.NewReader("http://ericsson.com\nIt is the home page"))
	line, _ := reader.ReadSlice('\n')
	fmt.Printf("the line is:%s\n", line)
	n, _ := reader.ReadSlice('\n')
	fmt.Printf("the line is:%s\n", line) //line changed!!!
	fmt.Println(string(n))

}

func Peek(reader *bufio.Reader) {
	line, _ := reader.Peek(14)
	fmt.Printf("%s\n", line)
	//time.Sleep(1)   //if uncomment this line, the line will cause ERROR
	fmt.Printf("%s\n", line)
}

func test_bufio_peek() {
	reader := bufio.NewReaderSize(strings.NewReader("http://studygolang.com.\t It is the home of gophers"), 14)
	go Peek(reader)
	go reader.ReadBytes('\t')
	time.Sleep(1e8)
}

func test_bufio_scanner() {
	scanner := bufio.NewScanner(os.Stdin)
	for scanner.Scan() {
		fmt.Println(scanner.Text()) //Println will add back the final '\n'
	}
	if err := scanner.Err(); err != nil {
		fmt.Fprintln(os.Stderr, "reading standard input:", err)
	}
}

func test_reflect() {
	var w io.Writer = os.Stdout
	t := reflect.TypeOf(w)
	fmt.Println(t)
	v := reflect.ValueOf(3)
	fmt.Println(v)          // 3
	fmt.Printf("%v\n", v)   // 3
	fmt.Println(v.String()) //"<int value>"
	fmt.Println(v.Type())   //int  ==v.Type().String()

	x := v.Interface()
	fmt.Printf("%d\n", x) //3
	i := x.(int)
	fmt.Printf("%d\n", i) //3
}

func var_len(arg ...string) string {
	//return fmt.Sprintf("%s+%s+%s", arg...) //cannot use arg (type []string) as type []interface {} in argument to fmt.Sprintf
	//change []string to []interface{}
	iargs := make([]interface{}, len(arg))
	for i, v := range arg {
		iargs[i] = v
	}
	return fmt.Sprintf("%s+%s+%s", iargs...)
}

func test_func_var_arg() {
	fmt.Println(var_len("abc", "123", " kkk b"))
}

func test_os_arg() {
	fmt.Println(os.Args)
	var cliName = flag.String("name", "nick", "Input your Name")
	var cliAge = flag.Int("age", 28, "Input Your Age")
	fmt.Println(cliName, cliAge)
}

var cliFlag int

func init() {
	fmt.Println("init flag")
	flag.IntVar(&cliFlag, "flagname", 1234, "Just for Demo")
}

var GoInitOrder = AnswerToLife() // init 1st

func AnswerToLife() int {
	fmt.Println("AnswerToLife")
	return 42
}

func init() {
	fmt.Println("init") // init 2nd
	GoInitOrder = 0
}

func main() { // init 3rd
	println("hello main, running on:", runtime.GOOS)
	//flag.Parse()
	//flag.Args()
	l := list.New()
	e4 := l.PushBack(4)
	e1 := l.PushFront(1)
	l.InsertBefore(3, e4)
	l.InsertAfter(2, e1)
	l.PushBack("aa")
	l.PushBack(false)
	for e := l.Front(); e != nil; e = e.Next() {
		fmt.Println(e.Value)
	}
	//if GoInitOrder == 0 { //test init: 1. import -> const -> var -> init() 2. could multi init(), exec in order
	//fmt.Println("It's all a lie.")
	//}
	//test_os_arg()
	//test_func_var_arg()
	//test_reflect()
	//test_bufio_scanner()
	//test_bufio_peek()
	//test_bufio_readslice()
	//test_same_package_in_other_go()
	//test_net()
	//test_Copy()
	//test_panic_defer()
	//test_slice()
	//test_context_cancel()
	//test_chan()
	//test_close_chan()
	//test_OO()
	//test_signal()
	//test_coroutine()
	//test_coroutine_sig_quit_and_wait()
	//test_coroutine_waitmany()
	//test_select()
	//test_fan()
	//test_timeout()
	//test_daisy_chain(1000)
	//test_sieve()
}

/*
func process(p *pdu.Body) bool {
	return false
}
*/
