#include "../main/cpp/MultiQuadTuple.h"
#include <iostream>
using namespace std;

#define L 1
#define M 1 
#define K 2 
#define N 1 
#define DEBUG false

void testLeftCompose(MultiQuadTuple<N, M> &f, BitVector<N> &x){ //C:L->N, f:N->M
	BitMatrix<L> C = BitMatrix<L>::randomMatrix(N << 6);

	BitVector<N> Cx = C.template operator*<N>(x);
	BitVector<M> f_Cx = f(Cx);

	MultiQuadTuple<L, M> fC = f*C;
	BitVector<M> fC_x = fC(x);

	cout << "LEFT COMPOSE TEST:" << endl << endl;

	cout << "f (C*x) = ";
	f_Cx.print();
	cout << endl;

	cout << "f*C (x) = ";
	fC_x.print();
	cout << endl;

	cout << "x = ";
	x.print();
	cout << endl;

	cout << "---------------------------------" << endl;
}

void testRightCompose(MultiQuadTuple<N, M> &f, BitVector<N> &x){ //f:N->M, D:M->K
	BitMatrix<M> D = BitMatrix<M>::randomMatrix(K << 6); 

	MultiQuadTuple<N, K> Df = f.rMult<K>(D); 
	BitVector<K> Df_x = Df(x);

	BitVector<M> fx = f(x);
	BitVector<K> D_fx = D.template operator*<K>(fx);

	cout << "RIGHT COMPOSE TEST:" << endl << endl;

	cout << "D * f(x) = ";
	D_fx.print();
	cout << endl;

	cout << "D*f(x) = ";
	Df_x.print();
	cout << endl;

	cout << "x = ";
	x.print();
	cout << endl;

	cout << "---------------------------------" << endl;
}

int main(int argc, char **argv) {
	MultiQuadTuple<N, M> f = MultiQuadTuple<N, M>::randomMultiQuadTuple();
	BitVector<N> x = BitVector<N>::randomVector();

	testLeftCompose(f, x);
	testRightCompose(f, x);
	return 0;
}