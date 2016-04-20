#ifndef SRC_UTIL_HASHSTORER_H_
#define SRC_UTIL_HASHSTORER_H_

#include <vector>

template <typename T>
class HashStorer {
public:
    HashStorer(const T& data)
        :fData(0), fDataCop(data), fHash(hashVector(fDataCop)) {

    }
	HashStorer(const T* data)
        :fData(data), fDataCop(), fHash(hashVector(*data)) {

	}
	HashStorer(const T* data, size_t hash)
        :fData(data), fDataCop(), fHash(hash) {

	}

	size_t hashVector(const T& xs) const
	{
		size_t res = 0;
		for (const typename T::value_type& x : xs) {
			res ^= std::hash<typename T::value_type>()(x) + 0x9e3779b9 + (res << 6) + (res >> 2);
		}
		return res;
	}

	virtual bool isEqual(const HashStorer<T>& rhs) const {
        return (*rhs.getData()) == (*getData());
	}

	virtual bool operator==(const HashStorer<T>& rhs) const {
		return rhs.isEqual(*this);
	}

	const T* getData() const {
        if (fData) return fData;
        return &fDataCop;
	}

	virtual ~HashStorer() {

	}

	const T* fData;
    const T fDataCop;
	size_t fHash;
};


namespace std {
template <typename T> struct hash<std::vector<T>>
{
	size_t operator()(const std::vector<T>& xs) const
	{
		size_t res = 0;
		for (const T& x : xs) {
			res ^= hash<T>()(x) + 0x9e3779b9 + (res << 6) + (res >> 2);
		}
		return res;
	}
};

template <typename T> struct hash<HashStorer<T>>
{
    size_t operator()(const HashStorer<T>& store) const
	{
		return store.fHash;
	}
};

}

#endif /* SRC_UTIL_HASHSTORER_H_ */
