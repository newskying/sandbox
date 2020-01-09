package calc

import "testing"

func TestCalc(t *testing.T) {
	result := Add(1, 2)
	if result != 3 {
		t.Fatal("testing Add: not pass: 1+2==3")
	}
	//t.Fatal("not implemented")
}
