#include "stdafx.h"
#include "v1store.h"
#include "utils.h"

v1store::v1store(const std::wstring &basePath)
	: binarystore(basePath, L"v1\\", 20)
{
}

v1store::~v1store()
{
}

std::wstring v1store::GetRangeFromHash(const SecureArrayT<BYTE> &hash)
{
	return ToHexString(hash.get(), hash.get() + 3).substr(0, 5);
}