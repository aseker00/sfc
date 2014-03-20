/*
 * fwkUtestStringTest.cpp
 *
 *  Created on: Feb 28, 2014
 *      Author: amit
 */

#ifndef SFC_FWK_UTEST_STRING_TEST_H
#define SFC_FWK_UTEST_STRING_TEST_H

#include <stdio.h>
#include "/mnt/shared/dev/users/amit/sfc/src/fwk/fwkString.h"

using namespace fwk;

int main(int argc, char **argv) {
	String s1(argv[1]);
	String s2(s1);
	String s3 = s2;

	printf("%i", s1 == s2);
	printf("%i", s1 == s3);
	return 0;
}

#endif


