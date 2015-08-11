#ifndef krypto_EncryptedSearchPublicKey_h
#define krypto_EncryptedSearchPublicKey_h

#include "PublicKey.h"
//#include <emscripten/bind.h>

template<unsigned int N, unsigned int L>
class EncryptedSearchPublicKey{
public:
	EncryptedSearchPublicKey(const PrivateKey<N, L> & pk) : 
	_pk(pk){}

	//h is generated by server, publicly available
	const BitVector<2*N> homomorphicHash(const MultiQuadTuple<2*N, N> & h, const BitVector<2*N> & t, const BitVector<2*N> & d) const{
		BridgeKey<N, L> bk(_pk);
		PublicKey<N, L> pub(bk);
		BitVector<2*N> result = _pk.encrypt(BitVector<N>::zeroVector());
		unsigned int count = 0;
		BitMatrix<N> C = h.getTransposedContributionMatrix();
		BitMatrix<N> Ai = BitMatrix<N>::squareZeroMatrix();
		BitMatrix<N> Ji = BitMatrix<N>::squareZeroMatrix();
		for(size_t i = 0; i < NN; ++i){
			Ai.zero();
			Ji.zero();
			Ji.template setCol<N>(i);
			for(size_t j = i; j < NN; ++j){
				Ai.template setCol<N>(j, C.getRow(count));
				++count;
			}
			BitVector<2*N> v1 = pub.homomorphicLMM(bk.getLMMZ(Ai), t);
			for(size_t j = 0; j < NN; ++j){
				Ai.template setCol<N>(j, C.getRow(count)); 
				++count;
			}
			BitVector<2*N> v2 = pub.homomorphicLMM(bk.getLMMZ(Ai), d);
			result = pub.homomorphicXOR(result, pub.homomorphicAND(pub.homomorphicLMM(bk.getLMMZ(Ji), t), pub.homomorphicXOR(v1, v2)));
		}
		for(size_t i = 0; i < NN; ++i){
			Ai.zero();
			Ji.zero();
			Ji.template setCol<N>(i);
			for(size_t j = i; j < NN; ++j){
				Ai.template setCol<N>(j, C.getRow(count)); 
				++count;
			}
			result = pub.homomorphicXOR(result, pub.homomorphicAND(pub.homomorphicLMM(bk.getLMMZ(Ji), d), pub.homomorphicLMM(bk.getLMMZ(Ai), d)));
		}
		result = pub.homomorphicXOR(result, _pk.encrypt(h.getConstantTerms()));
		return result;
	}

private:
	PrivateKey<N, L> _pk;
	static const unsigned int NN = N << 6;
	static const unsigned int twoNN = NN << 1;
};

/*
EMSCRIPTEN_BINDINGS(my_class_example) {
	class_<EncryptedSearchPublicKey>("EncryptedSearchPublicKey")
    .constructor<PrivateKey>()

    .function("incrementX", &MyClass::incrementX)
    .property("x", &MyClass::getX, &MyClass::setX)
    .class_function("getStringFromInstance", &MyClass::getStringFromInstance)
    ;
}
*/
#endif/* defined(__krypto__EncryptedSearchPublicKey__) */