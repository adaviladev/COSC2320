// with arrays
class Stack{
	int arr[];
	int n; // holds the # of elements
public:
	Stack(){
		arr = new int[MAX_SIZE];
		n = 0;
	}
	~Stack(){
		delete arr;
	}
	void push(int e){
		if(n == MAX_SIZE){
			cout << "full stack" << endl; // stack overflow
			return;
		} else {
			arr[n] = e;
			n++;
		}
	}
	void pop(){
		if(n != 0){
			cout << "Stack is empty." << endl; // stack underflow
			return;
		} else {
			n--;
		}
	}
	int top(){
		if(n == 0){
			cout << "Empty stack" << endl;
			return -1; // error code
		} else {
			return arr[n-1];
		}
	}
};