#include <iostream>
#include <string>
#include <sstream>
#include "simple_test.h"
using namespace std;

class BlockNode {
public:   enum BlockType { CLUSTER, RAM, IO, CLOCKCENTER };   
BlockNode(BlockType _t, int _x, int _y, int _p, bool _c) : 
	type(_t), x(_x), y(_y), pin(_p), isPin(_c) {
	switch (type) {
	case CLUSTER:
		output_signal = "cluster";
		break;
	case RAM:
		output_signal = "ram";
		break;
	case IO:
		output_signal = "io";
		break;
	case CLOCKCENTER:
		output_signal = "clockcenter";
		break;
	}
	output_signal = output_signal + "_" + to_string(x) + "_" + to_string(y);
	output_signal += isPin ? "_pin_" : "_class_";
	output_signal += to_string(pin);
}   
void printNode() const;   
void printNode(ostream& os) const;

private:  
	BlockType type;   
	int x, y;   
	int pin;  
	bool isPin;
	string output_signal;
};

void BlockNode::printNode() const { 
	cout << output_signal << endl;
}

void BlockNode::printNode(ostream& os) const { 
	os << output_signal << endl;
}

void checkOutput(const BlockNode& b, const string& req) {  
	ostringstream os;
	b.printNode(os);  
	CHECK_EQUAL(os.str(), req);
}
int  test_main() {
	//PINÀà
	auto ex01 = BlockNode{ BlockNode::CLUSTER,1,0,10,true };
	auto ex02 = BlockNode{ BlockNode::RAM,0,1,100,true };
	auto ex03 = BlockNode{ BlockNode::IO,1,0,10,true };
	auto ex04 = BlockNode{ BlockNode::CLOCKCENTER,0,1,100,true };
	//class
	auto ex05 = BlockNode{ BlockNode::CLUSTER,1,0,10,false };
	auto ex06 = BlockNode{ BlockNode::RAM,0,1,100,false };
	auto ex07 = BlockNode{ BlockNode::IO,1,0,10,false };
	auto ex08 = BlockNode{ BlockNode::CLOCKCENTER,0,1,100,false };

	string req01 = "cluster_1_0_pin_10\n";
	string req02 = "ram_0_1_pin_100\n";
	string req03 = "io_1_0_pin_10\n";
	string req04 = "clockcenter_0_1_pin_100\n";
	string req05 = "cluster_1_0_class_10\n";
	string req06 = "ram_0_1_class_100\n";
	string req07 = "io_1_0_class_10\n";
	string req08 = "clockcenter_0_1_class_100\n";

	checkOutput(ex01, req01);
	checkOutput(ex02, req02);
	checkOutput(ex03, req03);
	checkOutput(ex04, req04);
	checkOutput(ex05, req05);
	checkOutput(ex06, req06);
	checkOutput(ex07, req07);
	checkOutput(ex08, req08);
	return 0;
}