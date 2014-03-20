#include "../FwkString.h"
#include <stdio.h>

using namespace Fwk;

int main(char **argc, int argv) {
	String s1("amit");
	String s2 = "my name is amit and i am";
	String s3 = "my name is aamita and i am";
	int pos1 = s2.find(s1,0,s1.length());
	int pos2 = s3.find(s1,0,s1.length());
	String s4 = s2.substr(pos1, s2.length()-pos1);
	String s5 = s3.substr(pos2, s3.length()-pos2);
	printf("%d\t%d", s1.length(), s2.length());
}