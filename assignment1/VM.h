#ifndef VM_H
#define VM_H

#include "main.h"

bool stop_running;
unsigned int IP, excute_IP;
enum VariableType {NULL_,BOOL_,INT_,FLOAT_,ADDR_};
union RegisterValue {bool bool_value; int int_value; float float_value;int addr_value;};
vector<string> op_list;
vector<vector<string>> instruction_list;
vector<int> return_address_stack;
smatch reg_match;

string arithmetic_instruction[4] = {"Add", "Minus", "Mul", "Div"}; //all 2op
string comparision_instruction[6] = {"CmpEQ", "CompNE", "CmpLT", "CmpLE", "CmpGT","CmpGE"}; //all 2op
string logical_instruction[3] = {"Not", "And","Or"}; // "Not" 1op, the others 2op
string load_and_store_instruction[3] = {"Move","Load","Store"}; //all 2op
string sequence_control_instruction[5] = {"Return","Halt","Jump","Call","JumpIf"}; //"Return" "Halt" 0op, "Jump" Call 1op, "JumpIf" 2op
string input_ouput_instruction[2] = {"Input","Output"}; //all 0op

class Register;
bool sto_b(string&);
bool is_bool(string&);
bool is_dec(string&);
bool is_float(string&);
bool is_addr(string&);
bool is_regis(string&);
bool is_literal(string&);
bool is_src(string&);
VariableType get_value_type(string&);
bool is_valid_instruction(string&);
bool is_2operand_instruction(string&);
bool is_1operand_instruction_dst(string&);
bool is_1operand_instruction_src(string&);
bool is_0operand_instruction(string&);
void check_valid_syntax(vector<string>&);
void excute_arithmetic_instruction(vector<string>&);
void excute_comparision_instruction(vector<string>&);
void excute_logical_instruction(vector<string>&);
unsigned int addr_value_(string&);
void excute_load_and_store_instruction(vector<string>&);
void excute_sequence_control_instruction(vector<string>&);
void excute_input_ouput_instruction(vector<string>&);
void excute_instruction(vector<string>&);

class VM
{
public:
  VM();
  ~VM();
  void run(string filename);
};
#endif
