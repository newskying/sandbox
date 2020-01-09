package main

import (
	"fmt"
	"math"
	//"runtime"
	//"math/cmplx"
)

var (
	ToBe   bool       //= false
	MaxInt uint64     //= 1<<64 - 1
	z      complex128 //= cmplx.Sqrt(-5 + 12i)
	i      int        //= 42
	f      float64    //= float64(i)
	u      uint       //= uint(f)
	s      string     = "hello"
)

const (
	World = "shijie"
	Big   = 1 << 100
	Small = Big >> 99
	a     = 100
)

func needInt(x int) int { return x }
func needFload(x float64) float64 {
	return x
}

func test_for_if_switch() float64 {
	sum := 0
	for i := 0; i < 10; i++ {
		sum += i
	}
	fmt.Println(sum)

	sum = 1
	for sum < 1000 {
		sum += sum
	}
	fmt.Println(sum)

	if v := math.Pow(2, 8); v < 1024 {
		return v
	}
	return 100 //v //ERROR: undefined v
}

func pow(x, n, lim float64) float64 {
	if v := math.Pow(x, n); v < lim {
		return v
	} else {
		fmt.Printf("%g >= %g\n", v, lim)
	}
	// 这里开始就不能使用 v 了
	return lim
}

func test_print() {
	const f = "%T(%v)\n"
	fmt.Printf(f, ToBe, ToBe)
	fmt.Printf(f, MaxInt, MaxInt)
	fmt.Printf(f, z, z)
}

func test_big_small() {
	fmt.Println(needInt(Small))
	fmt.Println(needFload(Small))
	fmt.Println(needFload(Big))
	//fmt.Println(needInt(Big)) //ERROR: too big for Int
}

func test_defer() {
	fmt.Println("counting")
	for i = 0; i < 10; i++ {
		defer fmt.Println(i)
	}
	fmt.Println("done")
}

func test_pointer() {
	type Vertex struct {
		X, Y int
	}
	var v = Vertex{1, 2}
	var p = &v
	fmt.Println(v.X, v.Y)
	p.X = 3
	p.Y = 4
	fmt.Println(p.X, p.Y)
	fmt.Println(v.X, v.Y)
}

func printSlice(s string, x []int) {
	fmt.Printf("%s len=%d cap=%d %v\n", s, len(x), cap(x), x)
}

func test_slice() {
	a := make([]int, 5) //len, cap = 5, 5
	printSlice("a", a)
	b := make([]int, 0, 5) //len, cap = 0, 5
	printSlice("b", b)
	c := b[:2]
	printSlice("c", c) //len, cap = 2, 5
	d := c[2:5]
	printSlice("d", d) //len, cap = 3, 3
	var z []int        //len, cap = 0, 0
	printSlice("z", z)
}

type Shape interface {
	area() float64
	//round() float64
}

type Circle struct {
	radius float64
}

type Rectangle struct {
	width, height float64
}

func (c Circle) area() float64 {
	return math.Pi * c.radius * c.radius
}

func (r Rectangle) area() float64 {
	return r.width * r.height
}

func getArea(shape Shape) float64 {
	return shape.area()
}

func test_interface() {
	var c = Circle{2.0}
	var r = Rectangle{3.0, 4.1}
	fmt.Println(getArea(c), getArea(r))
}

func test_defer1() {
	var fs = [4]func(){}

	for i = 0; i < 4; i++ {
		defer fmt.Println("defer i = ", i)
		defer func() {
			fmt.Println("defer_closure i = ", i)
		}()
		fs[i] = func() { fmt.Println("closure i = ", i) }
	}

	for _, f := range fs {
		f()
	}
}

func init() {
	fmt.Println("hi ", ToBe)
}

func main() {
	fmt.Println("vim-go", "1", 2)
	//test_print()
	//test_big_small()
	//fmt.Println(test_for_if_switch())
	//fmt.Println(
	//pow(3, 2, 10),
	//pow(3, 3, 10),
	//)
	//fmt.Println(runtime.GOOS)
	//test_defer()
	//test_pointer()
	//test_slice()
	//test_interface()
	test_defer1()
}

func init() {
	fmt.Println("hello ", MaxInt)
}
