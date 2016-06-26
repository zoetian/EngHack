#include "stdtype.h"

static inline u64 write(u64 fd, void * buf, u64 len)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)1), "D"(fd), "S"(buf), "d"(len) : "rcx", "r11"
	);

	return ret;
}
static inline u64 writev(u64 fd, void * iov, u64 iovcnt)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)20), "D"(fd), "S"(iov), "d"(iovcnt) : "rcx", "r11"
	);

	return ret;
}
static inline u64 read(u64 fd, void * buf, u64 len)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)0), "D"(fd), "S"(buf), "d"(len) : "rcx", "r11"
	);

	return ret;
}
static inline u64 bind(u64 sockfd, const void * addr, u64 addrlen)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)49), "D"(sockfd), "S"(addr), "d"(addrlen) : "rcx", "r11"
	);

	return ret;
}
static inline u64 accept(u64 sockfd, const void * addr, u64 addrlen)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)43), "D"(sockfd), "S"(addr), "d"(addrlen) : "rcx", "r11"
	);

	return ret;
}
static inline u64 listen(u64 sockfd, u64 backlog)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)50), "D"(sockfd), "S"(backlog): "rcx", "r11"
	);

	return ret;
}
static inline u64 close(u64 fd)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)3), "D"(fd) : "rcx", "r11"
	);

	return ret;
}
static inline __attribute__((noreturn))  void exit(u64 code)
{
	asm volatile
	(
		"syscall\n\t"
		:: "a"((u64)60), "D"(code) : "rcx", "r11"
	);

	__builtin_unreachable();
}

static inline __attribute__((warn_unused_result)) u64 socket(u64 domain, u64 type, u64 protocol)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)41), "D"(domain), "S"(type), "d"(protocol) : "rcx", "r11"
	);

	return ret;
}
static inline u64 splice(u64 fd_in, void * off_in, u64 fd_out, void * off_out, u64 len, u64 flags)
{
	u64 ret;

	register u64 r10 asm ("%r10") = off_out;
	register u64 r8 asm ("%r8") = len;
	register u64 r9 asm ("%r9") = flags;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)275), "D"(fd_in), "S"(off_in), "d"(fd_out), "r"(r10), "r"(r8), "r"(r9) : "rcx", "r11"
	);

	return ret;
}
static inline void * brk(void * program_break)
{
	void * ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)12), "D"(program_break) : "rcx", "r11"
	);

	return ret;
}
static inline __attribute__((warn_unused_result)) u64 connect(u64 sockfd, const void * addr, u64 addrlen)
{
	u64 ret;

	asm volatile
	(
		"syscall\n\t"
		: "=a"(ret) : "a"((u64)42), "D"(sockfd), "S"(addr), "d"(addrlen) : "rcx", "r11"
	);

	return ret;
}

static inline void * alloc(void * p, u64 size)
{
	return brk((void *)(((upr)p & ~(4096 - 1)) + (size & ~(4096 - 1)) + 4096)) == p ? p : p + size;
}
static inline __attribute__((const)) void * align(void * a)
{
	void * b = (u64)a & ~7;
	return a == b ? a : a + 8;
}
static inline __attribute__((warn_unused_result)) u64 sc(void)
{
	return socket(2,1,0);
}
static const u8 uwaterloo_api[] = {2,0,0,80,129,97,208,19};
static const u8 server_addr[] = {2,0,0,80,0,0,0,0};

static bool get_json(u64 crawler,void * buf, u64 memory)
{

	void * bound;
	u64 balance;
	u64 bytes;

	do
	{
		bytes = read(crawler,buf,memory);
		memory -= bytes;

		if(memory == 0)
			return true;

		if(bytes)
		{
			bound = buf + bytes;
			while(buf < bound)
			{
				if(*(u8 *)buf == '{')
					balance++;
				else
				if(*(u8 *)buf == '}')
					balance--;
				buf++;
			}
		}
		else
			break;
	}
	while(balance);

	return false;
}

static void * itoa(u64 i, u8 * j)
{
	u8 * n = __builtin_alloca(16);
	u8 * m = n;

		do
			*m++ = i % 10 + 0x30;
		while(i /= 10);

		do
			*j++ = *--m;
		while(m > n);

		return j - 1;
}

static const u8 hexadecimal[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

static inline u64 istring(u64 i)
{
	return (hexadecimal[i >> 4] << 8) | hexadecimal[i & 0xf];
}

static inline void memcpy(void * dest, void * src, u64 len)
{
	asm volatile
	(
		"rep movsb\n\t"
		:: "D"(dest), "S"(src), "c"(len) : "cc"
	);
	asm volatile
	(
		"" ::: "rdi","rsi","rcx"
	);

	return;
}
static u32 timefhm(u64 i)
{
	i -= 0x00000030303a3030;
	return (i & 0xff) * 600 + ((i >> 8) & 0xff) * 60 + ((i >> 24) & 0xff) * 10 + ((i >> 32) & 0xff);
}
static void quicksort2(void * lo, void * hi, void ** stack)
{
	void ** sp = stack;
	do
	{
		u64 pivot = *(u64 *)hi;

		void * i = lo;
		void * j = lo;
		u64 k;

		while(j < hi)
		{
			if(*(u64 *)j <= pivot)
			{
				k = *(u64 *)j;
				*(u64 *)j = *(u64 *)i;
				*(u64 *)i = k;

				i += 8;

				k = *(u64 *)(j + 8);
				*(u64 *)(j + 8) = *(u64 *)i;
				*(u64 *)i = k;

				i += 8;
			}
			j += 16;
		}

		k = *(u64 *)hi;
		*(u64 *)hi = *(u64 *)i;
		*(u64 *)i = k;

		k = *(u64 *)(hi + 8);
		*(u64 *)(hi + 8) = *(u64 *)(i + 8);
		*(u64 *)(i + 8) = k;

		if(lo < (i - 16))
		{
			sp[0] = lo;
			sp[1] = i - 16;

			sp += 2;
		}

		if((i + 16) < hi)
			lo = i + 16;
		else
		if((void *)sp != (void *)stack)
		{
			lo = sp[-2];
			hi = sp[-1];

			sp -= 2;
		}
		else
			break;
	}
	while(true);

	return;
}
static inline u16 hex(u8 a)
{
	return hexadecimal[a >> 4] | (hexadecimal[a & 0xf] << 8);
}
static void quicksort12(void * lo, void * hi, void ** stack)
{
	void ** sp = stack;
	do
	{
		u64 pivot = *(u64 *)hi & 0xffffff;

		void * i = lo;
		void * j = lo;
		u64 k;

		while(j < hi)
		{
			if((*(u64 *)j & 0xffffff) <= pivot)
			{
				k = *(u64 *)j;
				*(u64 *)j = *(u64 *)i;
				*(u64 *)i = k;

				i += 8;
			}
			j += 8;
		}

		k = *(u64 *)hi;
		*(u64 *)hi = *(u64 *)i;
		*(u64 *)i = k;

		if(lo < (i - 8))
		{
			sp[0] = lo;
			sp[1] = i - 8;

			sp += 2;
		}

		if((i + 8) < hi)
			lo = i + 8;
		else
		if((void *)sp != (void *)stack)
		{
			lo = sp[-2];
			hi = sp[-1];

			sp -= 2;
		}
		else
			break;
	}
	while(true);

	return;
}
/*
static void itoa(u64 x, void * i)
{
	u64 y;

	y  = (x % 10 + 0x30) << 24 ;
	x /= 10;
	if(x == 0)
		goto ll;
	y |= (x % 10 + 0x30) << 16 ;
	x /= 10;
	if(x == 0)
		goto ll;
	y |= (x % 10 + 0x30) << 8;
	x /= 10;
	if(x == 0)
		goto ll;
	y |= (x % 10 + 0x30) << 9;
	ll:

	*(u32 *)i = y;
}
*/
struct lecture
{
	u64 room;
	u32 start;
	u32 fin;
	u8 weekdays;
};
/*
asm
(
	".global _start\n\t"
	"_start:\n\t"
	"and $~(4096 -1), %rsp\n\t"
	"add $4096, %rsp\n\t"
);
*/
__attribute__((noreturn)) __attribute__((no_reorder)) __attribute__((used)) void main(void)
{
	void * zero = brk(0);
	void * program_break = zero;

	void * buf = program_break;

	program_break = alloc(program_break,16777216);
	if(program_break == buf)
		exit(1);

	u64 crawler = sc();

	if(connect(crawler,(void *)uwaterloo_api,16) != 0)
		exit(2);

	const void * courses_request = "GET /v2/courses.json?key=230dfa91323a554b71b0ea8918e3e74a\r\n";

	write(crawler,courses_request,59);

	u64 balance = 0;
	u64 memory = 1048576;
	u64 bytes;
	u64 total = 0;
	void * i = buf;
	void * bound;

	do
	{
		bytes = read(crawler,i,memory);
		memory -= bytes;
		total += bytes;

		if(memory == 0)
			exit(3);

		if(bytes)
		{
			bound = i + bytes;
			while(i < bound)
			{
				if(*(u8 *)i == '{')
					balance++;
				else
				if(*(u8 *)i == '}')
					balance--;

				i++;
			}
		}
		else
			break;
	}
	while(balance);

	close(crawler);

	void * address_table = program_break;

	program_break = alloc(program_break,8 * 8192);
	if(program_break == buf)
		exit(1);

	i = buf + 107;
	bound = buf + total;
	void * j;
	void * k = buf;
	void ** l = address_table;

	*l = k;

	while(i++ < bound && *(u64 *)i != 0x5b3a226174616422);

	i += 41;
	do
	{
		j = i + 3;

		while(*(u8 *)j++ != '\"');

		u64 len = (void *)j - (void *)i;

		*(u64 *)k = *(u64 *)i;

		k += len;
		*(u8 *)(k - 1) = '/';

		i = j + 19;

		*(u64 *)k = *(u64 *)i;
		if(*(u8 *)(i + 3) == '\"')
		{
			k += 3;
			i += 14;
		}
		else
		if(*(u8 *)(i + 4) == '\"')
		{
			k += 4;
			i += 15;
		}
		else
		{
			/* Garbage data */
			k -= len;
			i += 15;
		}

		while(*(u8 *)i++ != '}');

		*l++ = k;
		i += 34;
	}
	while(i < bound);

	u64 * iovec = __builtin_alloca(16 * 5);

	iovec[0] = (u64)"GET /v2/courses/";
	iovec[1] = 16;

	/* Course ID */

	iovec[4] = (u64)"/schedule.json?key=230dfa91323a554b71b0ea8918e3e74a\r\n";
	iovec[5] = 53;

	void * schedule = k;
	i = schedule;
	bound = l - 1;
	l = address_table;

	struct lecture * n = k;

	j = buf + 1048576;
/*
	u64 objects = 0;
	u64 arrays = 0;
	
	*(u64 *)k = 0x0000006420726176;
	k += 5;

	*(u16 *)k = (u16)istring(arrays);
	k += 2;

	arrays += 1;

	*(u16 *)k = 0x5b3d;
	k += 2;
*/

	do
	{
		iovec[2] = (u64)l[0];
		iovec[3] = (u64)l[1] - (u64)l[0];

		crawler = sc();

		if(connect(crawler,(void *)uwaterloo_api,16) != 0)
			exit(2);

		writev(crawler,iovec,3);

		total = 0;
		balance = 0;
		memory = j - k;

		void * m;
		i = (void *)n;

		do
		{
			bytes = read(crawler,i,memory);
			memory -= bytes;
			total += bytes;

			if(memory == 0)
				exit(3);

			if(bytes)
			{
				void * b = i + bytes;
				while(i < b)
				{
					if(*(u8 *)i == '{')
						balance++;
					else
					if(*(u8 *)i == '}')
						balance--;

					i++;
				}
			}
			else
				break;
		}
		while(balance);

		i = (void *)n;
/*
		write(1,i,total);
		write(1,"\n",1);
*/
		i += 36;
		
		while(*(u64 *)i++ != 0x2273757461747322);

		i += 8;
/*
		write(1,i,20);
		write(1,"\n",1);
*/
		if(*(u64 *)i & 0xffffff != 0x303032)
		{
			close(crawler);
			l++;
			continue;
		}

		void * b = k + total;

		do
		{
			void * restore = k;

/*
			write(1,i,20);
			write(1,"\n",1);
*/
			while(*(u64 *)i++ != 0x745f747261747322)
			{
				if(!(i < b))
					goto out;
			}

			i += 12;

			if(*(u32 *)i == 0x6c6c756e)
				continue;
/*
			write(1,i,20);
			write(1,"\n",1);
*/

			i += 1;

			n[0].start = timefhm(*(u64 *)i & 0xffffffffff);

			i += 19;
/*
			write(1,i,20);
			write(1,"\n",1);
*/
			n[0].fin =  timefhm(*(u64 *)i & 0xffffffffff);

			i += 19;

			m = i;

			while(*(u8 *)++m != '\"');

			u64 len = m - i;
/*
			write(1,i,20);
			write(1,"\n",1);
*/
			u64 o = *(u64 *)i;
			u64 p = 0;

			do
			{
				cont:
				switch(o & 0xff)
				{
					case 'M':
						p |= 1 << 0;
					break;
					case 'T':
					{
						o = o >> 8;
						if((o & 0xff) == 'h')
							p |= 1 << 3;
						else
						{
							p |= 1 << 1;
							goto cont;
						}
					}
					break;
					case 'W':
						p |= 1 << 2;
					break;
					case 'F':
						p |= 1 << 4;
					break;
					default:
					goto out_s;
				}
				o = o >> 8;
			}
			while(true);
			out_s:

			n[0].weekdays = (u8)p;
/*
			u16 f = hexadecimal[p >> 4] | (hexadecimal[p & 0xf] << 8);
			write(1,&f,2);
			write(1,"\n",1);
*/
			i += len + 90;

			while(*(u64 *)i++ != 0x6f697461636f6c22);

			i += 22;

			if(*(u32 *)i == 0x6c6c756e)
			{
				k = restore;
				continue;
			}
/*
			write(1,i,20);
			write(1,"\n",1);
*/
			i++;

			m = i;

			while(*(u8 *)++m != '\"');

			len = m - i;

			u64 shift = (len << 3);
			n[0].room  = *(u64 *)i & ~(~(u64)0 << shift);

			i += len + 10;

			m = i;

			while(*(u8 *)++m != '\"');

			len = m - i;

			n[0].room |= (*(u64 *)i & ~(~(u64)0 << (len << 3))) << shift;

			i = m + 1;

			n++;
		}
		while(i < b);

		out:

		close(crawler);

		l++;
	}
	while(l < bound);

	bound = n;
	n = schedule;

	i = address_table;

	do
	{
		*(u64 *)i = n[0].room;
		*(void **)(i + 8) = n;
		i += 16;
		n++;
	}
	while(n < bound);

	quicksort2(address_table,i - 16,zero);

	bound = i;
	i = address_table;

	j = zero;

	do
	{
		u64 b = *(u64 *)i;
		i += 8;
		
		*(u64 *)j = b;
		j += 8;

		u64 ** ca = (void *)j;
		j += 8;

		k = j;

		n = *(void **)i;
		i += 8;

		*(u64 *)j = (u64)n[0].start | ((u64)n[0].weekdays << 24) | ((u64)n[0].fin << 32);

		j += 8;


		while(b == *(u64 *)i)
		{
			i += 8;

			n = *(void **)i;
			i += 8;

			*(u64 *)j = (u64)n[0].start | ((u64)n[0].weekdays << 24) | ((u64)n[0].fin << 32);
			j += 8;
		}

		*ca = j - k;
	}
	while(i < bound);

	bound = j;
	i = zero;
	void * divider = zero + 8388608;
	k = divider;

	do
	{
		j = i;
		i += 8;

		u64 len = *(u64 *)i;
		i += 8;
		void * hi = i + len;

		quicksort12(i,hi - 8,address_table);

		*(u64 *)k = 0x0000223a2272227b;
		k += 6;

		*(u64 *)k = *(u64 *)j;
		j += 8;

		while(*(u8 *)++k != 0);

		*(u64 *)k = 0x5b5b3a2274222c22;
		k += 8;

			void * t;
			u64 z;

			t = i;
			z = 0;

			*(u64 *)k = 0x0000303a2273227b;
			k += 6;

			do
			{
				u64 w = *(u64 *)t;
				t += 8;

				if((w >> 24) & (1 << 0))
				{
					*(u64 *)k = 0x0000003a2265222c;
					k += 5;

					k = itoa(w & 0xffffff,k);

					*(u16 *)k = 0x2c7d;
					k += 2;

					*(u64 *)k = 0x0000003a2273227b;
					k += 5;

					k = itoa(w >> 32,k);

				}
			}
			while((t + 8) < hi);

			*(u64 *)k = 0x0000003a2265222c;
			k += 5;

			*(u64 *)k = 0x5b2c5d7d30343431;
			k += 8;

			t = i;
			z = 0;

			*(u64 *)k = 0x0000303a2273227b;
			k += 6;

			do
			{
				u64 w = *(u64 *)t;
				t += 8;

				if((w >> 24) & (1 << 1))
				{
					*(u64 *)k = 0x0000003a2265222c;
					k += 5;

					k = itoa(w & 0xffffff,k);

					*(u16 *)k = 0x2c7d;
					k += 2;

					*(u64 *)k = 0x0000003a2273227b;
					k += 5;

					k = itoa(w >> 32,k);

				}
			}
			while((t + 8) < hi);

			*(u64 *)k = 0x0000003a2265222c;
			k += 5;

			*(u64 *)k = 0x5b2c5d7d30343431;
			k += 8;

			t = i;
			z = 0;

			*(u64 *)k = 0x0000303a2273227b;
			k += 6;

			do
			{
				u64 w = *(u64 *)t;
				t += 8;

				if((w >> 24) & (1 << 2))
				{
					*(u64 *)k = 0x0000003a2265222c;
					k += 5;

					k = itoa(w & 0xffffff,k);

					*(u16 *)k = 0x2c7d;
					k += 2;

					*(u64 *)k = 0x0000003a2273227b;
					k += 5;

					k = itoa(w >> 32,k);

				}
			}
			while((t + 8) < hi);

			*(u64 *)k = 0x0000003a2265222c;
			k += 5;

			*(u64 *)k = 0x5b2c5d7d30343431;
			k += 8;

			t = i;
			z = 0;

			*(u64 *)k = 0x0000303a2273227b;
			k += 6;

			do
			{
				u64 w = *(u64 *)t;
				t += 8;

				if((w >> 24) & (1 << 3))
				{
					*(u64 *)k = 0x0000003a2265222c;
					k += 5;

					k = itoa(w & 0xffffff,k);

					*(u16 *)k = 0x2c7d;
					k += 2;

					*(u64 *)k = 0x0000003a2273227b;
					k += 5;

					k = itoa(w >> 32,k);

				}
			}
			while((t + 8) < hi);

			*(u64 *)k = 0x0000003a2265222c;
			k += 5;

			*(u64 *)k = 0x5b2c5d7d30343431;
			k += 8;

			t = i;
			z = 0;

			*(u64 *)k = 0x0000303a2273227b;
			k += 6;

			do
			{
				u64 w = *(u64 *)t;
				t += 8;

				if((w >> 24) & (1 << 4))
				{
					*(u64 *)k = 0x0000003a2265222c;
					k += 5;

					k = itoa(w & 0xffffff,k);

					*(u16 *)k = 0x2c7d;
					k += 2;

					*(u64 *)k = 0x0000003a2273227b;
					k += 5;

					k = itoa(w >> 32,k);

				}
			}
			while((t + 8) < hi);

			*(u64 *)k = 0x0000003a2265222c;
			k += 5;

			*(u64 *)k = 0x5b2c5d7d30343431;
			k += 8;

		*(u32 *)(k - 2) = 0x002c7d5d;
		k += 1;

		i = hi;
	}
	while(i < bound);

	u64 len = (void *)k - (void *)schedule - 1;

	write(1,divider,(void *)k - (void *)divider);
	write(1,"\n",1);

	program_break = alloc(zero,len);

	const void * request = program_break;
	program_break = alloc(program_break,1024);

	if(request == program_break)
		exit(6);

	const void * http_header =

	"HTTP/1.1 200 OK\r\n"
	"Connection: close\r\n"
	"Content-Type: ";

	const void * content_length = "Content-Length: ";
	const void * CRLF = "\r\n\r\n";

	const void * text_html = "text/html\r\n";

	const u64 header_size = 36;

	const void * file_size = __builtin_alloca(8);

	iovec[0] = (u64)http_header;
	iovec[1] = 50;

	/* Content-Type */

	/*
	iovec[4] = (u64)content_length;
	iovec[5] = 16;

	iovec[6] = (u64)file_size;
	*/

	iovec[4] = (u64)CRLF;
	iovec[5] = 4;

	/* Content */

	iovec[8] = (u64)CRLF;
	iovec[9] = 4;

	const void * index_html = "Hello, World!";
	const u64 index_html_len = 13;

	u64 server = sc();

	if(bind(server, server_addr, 16) != 0)
		exit(5);

	if(listen(server,512) != 0)
		exit(8);

	do
	{
		u64 client = accept(server,0,0);

		u64 len = read(client,request,1024);
		if((s64)len <= 0)
		{
			close(client);
			continue;
		}
		if(len == 1024)
			exit(7);

		void * bound = len + request;
		u8 * i = request;

		write(1,request,len);
		if(*(u32 *)i != 0x20544547)
		{
			close(client);
			continue;
		}
		i += 4;

		void * url = i;

		do
		{
			i++;
			if(!((void *)i < bound))
			{
				close(client);
				continue;
			}
		}
		while(*i != ' ' && *i != '\r');

		u64 url_len = (void *)i - (void *)url;

		switch(url_len)
		{
			case 1:
			{
				if(*(u8 *)url == '/')
				{
					iovec[2] = (u64)text_html;
					iovec[3] = 11;

					iovec[6] = (u64)index_html;
					iovec[7] = index_html_len;
				}
			}
			break;
		}

		writev(client,(void *)iovec,5);

		close(client);
	}
	while(true);

	exit(0);

	__builtin_unreachable();
}
