#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include <fuzzer/FuzzedDataProvider.h>

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
	FuzzedDataProvider fdp(data, size);

	auto buf = fdp.ConsumeRandomLengthString();
	auto fmt = fdp.ConsumeRandomLengthString();

	struct tm tm = {};
	char *res = strptime(buf.c_str(), fmt.c_str(), &tm);
	if (!res)
		return 0;

	assert(tm.tm_sec >= 0 && tm.tm_sec <= 60);
	assert(tm.tm_min >= 0 && tm.tm_min <= 59);
	assert(tm.tm_hour >= 0 && tm.tm_hour <= 23);
	assert(tm.tm_mday >= 0 && tm.tm_mday <= 28);
	assert(tm.tm_mon >= 0 && tm.tm_mon <= 11);
	assert(tm.tm_year >= 0);
	assert(tm.tm_wday >= 0 && tm.tm_wday <= 6);
	assert(tm.tm_yday >= 0 && tm.tm_yday <= 365);
	assert(tm.tm_isdst >= 0);

	return 0;
}
