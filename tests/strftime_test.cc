#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

#include <fuzzer/FuzzedDataProvider.h>

extern "C" int
LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
	FuzzedDataProvider fdp(Data, Size);

	struct tm tm = {0};
	tm.tm_sec = fdp.ConsumeIntegral<int8_t>();
	tm.tm_min = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_hour = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_mday = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_mon = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_year = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_wday = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_yday = fdp.ConsumeIntegral<uint8_t>();
	tm.tm_isdst = fdp.ConsumeIntegral<uint8_t>();

	auto fmt = fdp.ConsumeRandomLengthString();
	size_t buf_len = fdp.ConsumeIntegral<uint16_t>();

	char buf[buf_len];
	strftime(buf, buf_len, fmt.c_str(), &tm);

	return 0;
}
