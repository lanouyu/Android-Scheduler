#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>

void delay(unsigned int s)
{
	unsigned int x, y, z, a;
	for (x=s; x>0; --x)
		for (y=110000; y>0; --y)
			a = ldexp(3413.341,x);
}

int main()
{
	struct timeval start, end;
    unsigned int interval;

    gettimeofday(&start, NULL);
    delay(1000);
    gettimeofday(&end, NULL);
    interval = 1000000*(end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec);

	printf("EXECUTION TIME: %f seconds.\n", interval/1000000.0);
	return 0;
}
