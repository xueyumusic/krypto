#ifndef krypto_EncryptedSearchPublicKey_h
#define krypto_EncryptedSearchPublicKey_h

#include "PublicKey.h"
#include "EncryptedSearchPrivateKey.h"

template<unsigned int N, unsigned int L>
class EncryptedSearchPublicKey{
public:

	EncryptedSearchPublicKey(const EncryptedSearchPrivateKey<N, L> & rk, const BridgeKey<N, L> & bk):
	_rk(rk), _bk(bk), _h(rk.getHash()){}

	const BitVector<2*N> homomorphicHash(const BitVector<2*N> & t, const BitVector<2*N> & d) const{
		PublicKey<N, L> pub(_bk);
		BitVector<2*N> result = _rk.getS();
		unsigned int count = 0;
		BitMatrix<N> C = _h.getTransposedContributionMatrix();
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
			BitVector<2*N> v1 = pub.homomorphicLMM(_bk.getLMMZ(Ai), t);
			for(size_t j = 0; j < NN; ++j){
				Ai.template setCol<N>(j, C.getRow(count)); 
				++count;
			}
			BitVector<2*N> v2 = pub.homomorphicLMM(_bk.getLMMZ(Ai), d);
			result = pub.homomorphicXOR(result, pub.homomorphicAND(pub.homomorphicLMM(_bk.getLMMZ(Ji), t), pub.homomorphicXOR(v1, v2)));
		}
		for(size_t i = 0; i < NN; ++i){
			Ai.zero();
			Ji.zero();
			Ji.template setCol<N>(i);
			for(size_t j = i; j < NN; ++j){
				Ai.template setCol<N>(j, C.getRow(count)); 
				++count;
			}
			result = pub.homomorphicXOR(result, pub.homomorphicAND(pub.homomorphicLMM(_bk.getLMMZ(Ji), d), pub.homomorphicLMM(_bk.getLMMZ(Ai), d)));
		}
		result = pub.homomorphicXOR(result, _rk.getV());
		return result;
	}

	const BitVector<N> getAddress(const BitVector<2*N> & t, const BitVector<2*N> & d) const{
		BitVector<2*N> E_f = homomorphicHash(t, d);
		//TODO: compute E_g from h_2,h_1,h_0
		//TODO: compute result using D_g and E_g
		return BitVector<N>::zeroVector(); //TODO
	}

private:
	BridgeKey<N, L> _bk;
	EncryptedSearchPrivateKey<N, L> _rk;
	MultiQuadTuple<2*N,N> _h;
	static const unsigned int NN = N << 6;
	static const unsigned int twoNN = NN << 1;
};

#endif/* defined(__krypto__EncryptedSearchPublicKey__) */