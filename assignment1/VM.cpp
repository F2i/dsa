#include "VM.h"


class Register
{
	private:
		RegisterValue res_value;
		VariableType res_type;
		
	public:
		Register(){res_value.float_value = 0, res_type = NULL_;}
		
		void set_value_float(const float &a){res_value.float_value = a;res_type = FLOAT_;}
		void set_value_int(const int &a){res_value.int_value = a;res_type = INT_;}
		void set_value_bool(const bool &a){res_value.bool_value = a;res_type = BOOL_;}
		void set_value_addr(const int &a){res_value.addr_value = a;res_type = ADDR_;}
		
		float get_value_float(){return res_value.float_value;};
		int get_value_int(){return res_value.int_value;};
		bool get_value_bool(){return res_value.bool_value;};
		int get_value_addr(){return res_value.addr_value;};
		
		VariableType f_res_type(){return res_type;};
		
		void empty_regis(){res_value.float_value = 0; res_type = NULL_;};
};

Register* p_Res;
Register* p_StackMemory;
///func
//bool is_func(string[], int, string &);
bool sto_b(string& operand)
{
	if(operand == "true")return true;
	return false;
}
//----------------------------------------------------------operand_type
bool is_bool(string& operand)
{
	return regex_match(operand,regex("true|false"));
}

bool is_dec(string& operand)
{
	return regex_match(operand,regex("(0|([1-9]\\d*))"));
}

bool is_float(string& operand)
{
	return regex_match(operand,regex("(0.\\d+|([1-9]\\d*)\\.\\d+)"));
}


bool is_addr(string& operand)
{
	return regex_match(operand, regex("0A|[1-9]\\d*A")) && (stoi(operand.substr(0,operand.length()-1)) < 65536);
}

//// abtract operand type
bool is_regis(string& operand)
{
	
	return regex_match(operand, regex("R[1-9]|R1[0-5]"));
}

bool is_literal(string& operand)
{
	return is_bool(operand) || is_dec(operand) || is_float(operand) ||  is_addr(operand);
}

///src or des
bool is_src(string& operand)
{
	return is_literal(operand) || is_regis(operand);
}

///----------------------------------------------------------value_type
VariableType get_value_type(string& operand)
{
	if(is_bool(operand)) return BOOL_;
	if(is_dec(operand)) return INT_;
	if(is_float(operand)) return FLOAT_;
	if(is_addr(operand)) return ADDR_;
	if(is_regis(operand)) return (p_Res-1+stoi(operand.substr(1,operand.length()-1)))->f_res_type();
	return NULL_;
}

///----------------------------------------------------------token_instruction

bool is_valid_instruction(string& instruction)
{
	return regex_match(instruction,reg_match, regex("([^ ]+) ([^ ]+), ([^ ]+)"))||regex_match(instruction,reg_match, regex("([^ ]+) ([^ ]+)"))||regex_match(instruction,reg_match, regex("([^ ]+)"));
}
//bool is_des(string& operand);
bool is_2operand_instruction(string& meme_operator)
{
	for(int i = 0; i < 4;i++)
	{
		if(meme_operator == arithmetic_instruction[i]) return true;
	}
	for(int i = 0; i < 6;i++)
	{
		if(meme_operator == comparision_instruction[i]) return true;
	}
	for(int i = 1; i < 3;i++)
	{
		if(meme_operator == logical_instruction[i]) return true;
	}
	for(int i = 0; i < 3;i++)
	{
		if(meme_operator == load_and_store_instruction[i]) return true;
	}
	if(meme_operator == sequence_control_instruction[4]) return true;
	return false;
}

bool is_1operand_instruction_dst(string& meme_operator)
{
	if(meme_operator == logical_instruction[0]) return true;
	if(meme_operator == input_ouput_instruction[0]) return true;
	return false;
}


bool is_1operand_instruction_src(string& meme_operator)
{
	for(int i = 2; i < 4;i++)
	{
		if(meme_operator == sequence_control_instruction[i]) return true;
	}
	if(meme_operator == input_ouput_instruction[1]) return true;
	return false;
}


bool is_0operand_instruction(string& meme_operator)
{
	for(int i = 0; i < 2;i++)
	{
		if(meme_operator == sequence_control_instruction[i]) return true;
	}
	return false;
}

void check_valid_syntax(vector<string>& op_list)
{
	if(op_list.size() == 3)
	{
		if(!is_2operand_instruction(op_list[0])) throw InvalidInstruction(IP); // cout << "wtf_instruction"<< IP << endl; // 
		else
		{
			if(!(is_regis(op_list[1]) && is_src(op_list[2]))) throw InvalidOperand(IP); //cout << "wtf_operand"<< IP << endl; // 
		} 
	}
	
	if(op_list.size() == 2)	
	{
		if(!is_1operand_instruction_dst(op_list[0]) && !is_1operand_instruction_src(op_list[0])) throw InvalidInstruction(IP); //cout << "wtf_instruction"<< IP << endl;// 
		else
		{
			if(is_1operand_instruction_dst(op_list[0]))
			{
				if(!is_regis(op_list[1])) throw InvalidOperand(IP); //cout << "wtf_operand" << IP<< endl; //
			}
			else
			{
				if(!is_src(op_list[1])) throw InvalidOperand(IP); //cout << "wtf_operand" << IP<< endl; //
			}
		}
	}
	if(op_list.size() == 1)
	{
		if(!is_0operand_instruction(op_list[0])) throw InvalidInstruction(IP); //cout << "wtf_instruction" << IP << endl; // 
	}
	//return false;
}

void excute_arithmetic_instruction(vector<string>& op_list)
{
	VariableType type1,type2;
	type1 = get_value_type(op_list[1]);
	type2 = get_value_type(op_list[2]);
	if(type1 != INT_ && type1 != FLOAT_ ) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type" << endl; // 
	if(type2 != INT_ && type2 != FLOAT_ ) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type" << endl; // 
	float op1,op2;
	int index;
	//cal indenx
	//cal op2
	if(is_regis(op_list[2]))
	{
		index = stoi(op_list[2].substr(1,op_list[2].length()-1))-1;
		if(type2 == INT_) op2 = (p_Res+index)->get_value_int();
		else op2 = (p_Res+index)->get_value_float();
	}
	else op2 = stof(op_list[2]);
	//cal op1
	index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
	if(type1 == INT_) op1 = (p_Res+index)->get_value_int();
	else op1 = (p_Res+index)->get_value_float();
	//perform ins
	if(op_list[0] == arithmetic_instruction[0])	op1 += op2;
	else if(op_list[0] == arithmetic_instruction[1])	op1 -= op2;
	else if(op_list[0] == arithmetic_instruction[2])	op1 *= op2;
	else
	{
		if(op2 == (float)0) throw DivideByZero(excute_IP); // cout << "wtf_devision_zero" << endl; // 
		else op1 /= op2;
	}
	if(type1 == INT_ && type2 == INT_ )	(p_Res+index)->set_value_int((int)op1);
	else (p_Res+index)->set_value_float(op1);
}


void excute_comparision_instruction(vector<string>& op_list)
{
	VariableType type1,type2;
	type1 = get_value_type(op_list[1]);
	type2 = get_value_type(op_list[2]);
	if(type1 != INT_ && type1 != FLOAT_ && type1 != BOOL_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_comparion" << endl; // 
	if(type2 != INT_ && type2 != FLOAT_ && type2 != BOOL_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_comparion" << endl; // 
	if((type1 == BOOL_ && type2 != BOOL_) || (type1 != BOOL_ && type2 == BOOL_)) throw TypeMismatch(excute_IP); // cout << "wtf_mismatch_type_comparion" << endl;
	//
	float op1,op2;
	int index;
	//cal op2
	if(is_regis(op_list[2]))
	{
		index = stoi(op_list[2].substr(1,op_list[2].length()-1))-1;
		if(type2 == INT_) op2 = (p_Res+index)->get_value_int();
		else if (type2 == BOOL_) op2 = (p_Res+index)->get_value_bool();
		else op2 = (p_Res+index)->get_value_float();
	}
	else 
	{
		if(type2 == BOOL_) op2 = sto_b(op_list[2]);
		else op2 = stof(op_list[2]);
	}
	//cal op1
	index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
	if(type1 == INT_) op1 = (p_Res+index)->get_value_int();
	else if (type1 == BOOL_) op1 = (p_Res+index)->get_value_bool();
	else op1 = (p_Res+index)->get_value_float();
	//perform ins
	if(op_list[0] == comparision_instruction[0]) (p_Res+index)->set_value_bool(op1 == op2);
	else if(op_list[0] == comparision_instruction[1]) (p_Res+index)->set_value_bool(op1 != op2);
	else if(op_list[0] == comparision_instruction[2]) (p_Res+index)->set_value_bool(op1 < op2);
	else if(op_list[0] == comparision_instruction[3]) (p_Res+index)->set_value_bool(op1 <= op2);
	else if(op_list[0] == comparision_instruction[4]) (p_Res+index)->set_value_bool(op1 > op2);
	else (p_Res+index)->set_value_bool(op1 >= op2);
}

void excute_logical_instruction(vector<string>& op_list)
{
	VariableType type1;
	type1 = get_value_type(op_list[1]);
	if(type1 != BOOL_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_logical" << endl; //
	int index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
	if(op_list[0] == logical_instruction[0]) (p_Res+index)->set_value_bool(!((p_Res+index)->get_value_bool()));
	else
	{
		if(get_value_type(op_list[2]) != BOOL_) throw TypeMismatch(excute_IP);
		bool value;
		if(is_regis(op_list[2]))
		{
			int index2;
			index2 = stoi(op_list[2].substr(1,op_list[2].length()-1))-1;
			value = (p_Res+index2)->get_value_bool();
		}
		else value = sto_b(op_list[2]);
		if(op_list[0] == logical_instruction[1]) value = value && ((p_Res+index)->get_value_bool());
		else value = value || ((p_Res+index)->get_value_bool());
		(p_Res+index)->set_value_bool(value);
	}
}

unsigned int addr_value_(string& operand)
{
	if(is_regis(operand))
	{
		int index_ = stoi(operand.substr(1,operand.length()-1))-1;
		return (p_Res+index_)->get_value_addr();
	}
	else return stoi(operand.substr(0,operand.length()-1));
}

void excute_load_and_store_instruction(vector<string>& op_list)
{
	int index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
	if(op_list[0] == load_and_store_instruction[0])
	{	
		if(is_regis(op_list[2]))
		{
			int index2 = stoi(op_list[2].substr(1,op_list[2].length()-1))-1;
			*(p_Res+index) = *(p_Res+index2);
		}
		else if(get_value_type(op_list[2]) == BOOL_) (p_Res+index)->set_value_bool(sto_b(op_list[2]));
		else if(get_value_type(op_list[2]) == INT_)	(p_Res+index)->set_value_int(stoi(op_list[2]));
		else if(get_value_type(op_list[2]) == FLOAT_) (p_Res+index)->set_value_float(stof(op_list[2]));
		else if(get_value_type(op_list[2]) == ADDR_) (p_Res+index)->set_value_addr(stoi(op_list[2].substr(0,op_list[2].length()-1)));
	}
	
	else if(op_list[0] == load_and_store_instruction[1])
	{
		if(get_value_type(op_list[2]) != ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_load" << endl; // 
		unsigned int addr_value = addr_value_(op_list[2]);
		*(p_Res+index) = *(p_StackMemory+addr_value);
	}
	
	else
	{
		if(get_value_type(op_list[1]) != ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_store" << endl; // 
		if(get_value_type(op_list[2]) == ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_store" << endl;
		unsigned int addr_value = (p_Res+index)->get_value_addr();
		if(is_regis(op_list[2]))
		{
			int index2 = stoi(op_list[2].substr(1,op_list[2].length()-1))-1;
			*(p_StackMemory+addr_value) = *(p_Res+index2);
		}
		else if(get_value_type(op_list[2]) == BOOL_) (p_StackMemory+addr_value)->set_value_bool(sto_b(op_list[2]));
		else if(get_value_type(op_list[2]) == INT_) (p_StackMemory+addr_value)->set_value_int(stoi(op_list[2]));
		else if(get_value_type(op_list[2]) == FLOAT_) (p_StackMemory+addr_value)->set_value_float(stof(op_list[2]));
	}
}

void excute_sequence_control_instruction(vector<string>& op_list)
{
	unsigned int addr_value;
	if(op_list[0] == sequence_control_instruction[0])
	{
		IP = return_address_stack[return_address_stack.size() - 1];
		return_address_stack.pop_back();
	}
	
	else if(op_list[0] == sequence_control_instruction[1]) stop_running = true;
	
	else if(op_list[0] == sequence_control_instruction[2])
	{
		if(get_value_type(op_list[1]) != ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_jump" << endl; // 
		addr_value = addr_value_(op_list[1]);
		if(addr_value >= instruction_list.size()) throw InvalidDestination(excute_IP); //cout << "wtf_full_destination" << endl; // 
		IP = addr_value;
	}
	
	else if(op_list[0] == sequence_control_instruction[3])
	{
		if(get_value_type(op_list[1]) != ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_call" << endl; // 
		addr_value = addr_value_(op_list[1]);
		if(addr_value >= instruction_list.size()) throw InvalidDestination(excute_IP); //cout << "wtf_full_destination" << endl; // 
		if(return_address_stack.size() < 1000)	return_address_stack.push_back(IP);
		else throw StackFull(excute_IP); //cout << "wtf_full_stack" << endl; // 
		IP = addr_value;
	}
	
	else
	{
		if(get_value_type(op_list[1]) != BOOL_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_jumpif" << endl; // 
		if(get_value_type(op_list[2]) != ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_jumpif" << endl; // 
		int index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
		if((p_Res+index)->get_value_bool())
		{
			addr_value = addr_value_(op_list[2]);
			if(addr_value >= instruction_list.size()) throw InvalidDestination(excute_IP); //cout << "wtf_full_destination" << endl; // 
			IP = addr_value;
		}
	}
}

void excute_input_ouput_instruction(vector<string>& op_list)
{
	if(op_list[0] == input_ouput_instruction[0])
	{
		string tmp;
		cin >> tmp;
		int index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
		if(get_value_type(tmp) == BOOL_) (p_Res+index)->set_value_bool(sto_b(tmp));
		else if(get_value_type(tmp) == INT_) (p_Res+index)->set_value_int(stoi(tmp));
		else if(get_value_type(tmp) == FLOAT_) (p_Res+index)->set_value_float(stof(tmp));
		else throw InvalidInput(excute_IP); // cout << "mismatch_type_input" << endl; //
	}
	else
	{
		if(get_value_type(op_list[1]) != BOOL_ && get_value_type(op_list[1]) != INT_ && get_value_type(op_list[1]) != FLOAT_ && get_value_type(op_list[1]) != ADDR_) throw TypeMismatch(excute_IP); //cout << "wtf_mismatch_type_ouput" << endl; // 
		else
		{
			if(is_regis(op_list[1]))
			{
				int index = stoi(op_list[1].substr(1,op_list[1].length()-1))-1;
				if(get_value_type(op_list[1]) == BOOL_)	 {if((p_Res+index)->get_value_bool()) cout << "true"; else cout << "false";}
				else if(get_value_type(op_list[1]) == INT_)	 cout << (p_Res+index)->get_value_int();
				else if(get_value_type(op_list[1]) == FLOAT_) cout << (p_Res+index)->get_value_float();
				else if(get_value_type(op_list[1]) == ADDR_) cout << (p_Res+index)->get_value_addr()<<"A";
			}
			else cout << op_list[1];
		}
	}
}

void excute_instruction(vector<string>& op_list)
{
	bool scan_ins =false;
	
	for(int i = 0; i < 4 && !scan_ins; i++)
	{
		if(op_list[0]==arithmetic_instruction[i])
		{
			excute_arithmetic_instruction(op_list);
			scan_ins = true;
		} 
	}
	
	for(int i = 0; i < 6 && !scan_ins; i++)
	{
		if(op_list[0]==comparision_instruction[i]) 
		{
			excute_comparision_instruction(op_list);
			scan_ins = true;
		}
	}
	
	for(int i = 0; i < 3 && !scan_ins; i++)
	{
		if(op_list[0]==logical_instruction[i]) 
		{
			excute_logical_instruction(op_list);
			scan_ins = true;
		}
	}
	
	for(int i = 0; i < 3 && !scan_ins; i++)
	{
		if(op_list[0]==load_and_store_instruction[i]) 
		{
			excute_load_and_store_instruction(op_list);
			scan_ins = true;
		}
	}
	
	for(int i = 0; i < 5 && !scan_ins; i++)
	{
		if(op_list[0]==sequence_control_instruction[i]) 
		{
			excute_sequence_control_instruction(op_list);
			scan_ins = true;
		}
	}
	
	for(int i = 0; i < 2 && !scan_ins; i++)
	{
		if(op_list[0]==input_ouput_instruction[i]) 
		{
			excute_input_ouput_instruction(op_list);
			scan_ins = true;
		}
	}
}

//get operand type
VM::VM()
{
	p_Res = new Register[15];
	p_StackMemory = new Register[65536];
}

VM::~VM()
{
	delete[] p_Res;
	delete[] p_StackMemory;
	vector<string>().swap(op_list);
	vector<vector<string>>().swap(instruction_list);
	vector<int>().swap(return_address_stack);
}

void VM::run(string filename)
{
	
	ifstream fileInput(filename);
	IP = 0;
	if (fileInput.is_open()) 
	{
	    string line;
	    while (getline(fileInput, line)) {
	        // 
	        if(!is_valid_instruction(line)) throw InvalidInstruction(IP); //cout << "wtf_instruction" << IP << endl; // 
	        op_list.clear();
	        for(unsigned int i = 1;i < reg_match.size();i++)
	        {
	        	op_list.push_back(reg_match[i]);
			}
			check_valid_syntax(op_list);
			instruction_list.push_back(op_list);
			IP++;
	    }
	    fileInput.close();
	}
	IP = 0;
	stop_running = false;
	while(IP < instruction_list.size() && !stop_running)
	{
		excute_IP = IP;
		IP++;
		excute_instruction(instruction_list[excute_IP]);
	}
}


