#include <string>
#include <gtest/gtest.h>

#include "boolexpr.h"
//* 
//  ** *
//STRING * *
//  ** *
//*     
TEST(STRING, test_string1) {
	EXPECT_EQ(std::string(BooleanExpression("x1 v x2")), "(x1 v x2)");
}
//*     
TEST(STRING, test_string14) {
	EXPECT_EQ(std::string(BooleanExpression("x1 v x2").cnf()), "(x2 v x1)");
}
//*     
TEST(STRING, test_string13) {
	EXPECT_EQ(std::string(BooleanExpression("x1 v x2").dnf()), "(x2 & ~x1) v (~x2 & x1) v (x2 & x1)");
}

TEST(STRING, test_string2) {
	EXPECT_EQ(std::string(BooleanExpression("x1 & x2")), "(x1 & x2)");
}


TEST(STRING, test_string15) {

	EXPECT_EQ(std::string(BooleanExpression("x1 & x2").dnf()), "(x2 & x1)");

}

TEST(STRING, test_string16) {

	EXPECT_EQ(std::string(BooleanExpression("x1 & x2").cnf()), "(x2 v x1) & (~x2 v x1) & (x2 v ~x1)");

}

TEST(STRING, test_string3) {
	EXPECT_EQ(std::string(BooleanExpression("x1 | x2")), "(x1 | x2)");

}


TEST(STRING, test_string17) {

	EXPECT_EQ(std::string(BooleanExpression("x1 | x2").cnf()), "(~x2 v ~x1)");

}


TEST(STRING, test_string18) {

	EXPECT_EQ(std::string(BooleanExpression("x1 | x2").dnf()), "(~x2 & ~x1) v (x2 & ~x1) v (~x2 & x1)");

}


TEST(STRING, test_string19) {

	EXPECT_EQ(std::string(BooleanExpression("x1 | x2").zhegalkin()), "1 + x2 & x1");

}


TEST(STRING, test_string4) {

	EXPECT_EQ(std::string(BooleanExpression("x1 < x2")), "(x1 < x2)");

}


TEST(STRING, test_string20) {

	EXPECT_EQ(std::string(BooleanExpression("x1 < x2").cnf()), "(~x2 v x1)");

}


TEST(STRING, test_string21) {

	EXPECT_EQ(std::string(BooleanExpression("x1 < x2").dnf()), "(~x2 & ~x1) v (~x2 & x1) v (x2 & x1)");

}


TEST(STRING, test_string22) {

	EXPECT_EQ(std::string(BooleanExpression("x1 < x2").zhegalkin()), "1 + x2 + x2 & x1");

}
TEST(STRING, test_string5) {

	EXPECT_EQ(std::string(BooleanExpression("x1 > x2")), "(x1 > x2)");

}
TEST(STRING, test_string23) {

	EXPECT_EQ(std::string(BooleanExpression("x1 > x2").cnf()), "(x2 v ~x1)");

}

TEST(STRING, test_string24) {

	EXPECT_EQ(std::string(BooleanExpression("x1 > x2").dnf()), "(~x2 & ~x1) v (x2 & ~x1) v (x2 & x1)");

}

TEST(STRING, test_string25) {

	EXPECT_EQ(std::string(BooleanExpression("x1 > x2").zhegalkin()), "1 + x1 + x2 & x1");

}
TEST(STRING, test_string6) {
	EXPECT_EQ(std::string(BooleanExpression("x1 = x2")), "(x1 = x2)");

}


TEST(STRING, test_string26) {

	EXPECT_EQ(std::string(BooleanExpression("x1 = x2").cnf()), "(~x2 v x1) & (x2 v ~x1)");

}

TEST(STRING, test_string27) {

	EXPECT_EQ(std::string(BooleanExpression("x1 = x2").dnf()), "(~x2 & ~x1) v (x2 & x1)");

}

TEST(STRING, test_string28) {

	EXPECT_EQ(std::string(BooleanExpression("x1 = x2").zhegalkin()), "1 + x2 + x1");

}
TEST(STRING, test_string7) {
	EXPECT_EQ(std::string(BooleanExpression("x1 ^ x2")), "(x1 ^ x2)");

}
TEST(STRING, test_string29) {

	EXPECT_EQ(std::string(BooleanExpression("x1 ^ x2").cnf()), "(~x2 v x1) & (x2 v ~x1) & (~x2 v ~x1)");

}
TEST(STRING, test_string30) {

	EXPECT_EQ(std::string(BooleanExpression("x1 ^ x2").dnf()), "(~x2 & ~x1)");

}
TEST(STRING, test_string31) {

	EXPECT_EQ(std::string(BooleanExpression("x1 ^ x2").zhegalkin()), "1 + x2 + x1 + x2 & x1");

}

TEST(STRING, test_string8) {

	EXPECT_EQ(std::string(BooleanExpression("~x1")), "~x1");

}

TEST(STRING, test_string9) {

	EXPECT_EQ(std::string(BooleanExpression("~x1").cnf()), "(~x1)");

}


TEST(STRING, test_string10) {

	EXPECT_EQ(std::string(BooleanExpression("~x1").dnf()), "(~x1)");

}


TEST(STRING, test_string12) {

	EXPECT_EQ(std::string(BooleanExpression("~x1").zhegalkin()), "1 + x1");

}

TEST(Test, TEST_FIRST) {

	BooleanExpression value("x1 v x2");
	std::string result_first = "(x2 v x1)";

	EXPECT_EQ(value.cnf(), result_first);
}

TEST(Test, TEST_SECOND) {

	BooleanExpression value("x1 & x2");
	std::string result_first = "(x2 & x1)";

	EXPECT_EQ(value.dnf(), result_first);
}
