#include "tinyC3_22CS30056_22CS30015_translator.h"

// This is the file to generate the symbol table and print the tokens, parse tree and the 3-address code.
// Roll No: 22CS30056 Sumit Kumar
// Roll No: 22CS30015 Aviral Singh

/*
	The use of te global variables is as follows:
		1. quad_array: It is used to store the quads.
		2. current_table: It is used to store the current table.
		3. global_table: It is used to store the global table.
		4. current_symbol: It is used to store the current symbol.
		5. current_type: It is used to store the current type.
		6. table_count: It is used to store the table count.
		7. temp_count: It is used to store the temporary count.
*/
vector<Quad *> quad_array{};
SymbolTable *current_table{}, *global_table{};
Symbol *current_symbol{};
SymbolType::SymbolEnum current_type{};
int table_count{}, temp_count{};

/*
	The constructor for the SymbolType.
*/
SymbolType::SymbolType(SymbolEnum type, SymbolType *array_type, size_t width): type(type), width(width), array_type(array_type) {}


/*
	The function to get the size of the symbol.
	This function manually return the type of the of the Symbol defined in the SymbolType.
	For type:
		1. INT: It returns 4.
		2. PTR: It returns 4, irrespective of the type of pointer.
		3. CHAR: It returns 1.
		4. FLOAT: It returns 8.
		5. ARRAY: It returns the width of the array multiplied by the size of the array type.
*/
size_t SymbolType::getSize(){
	switch (this->type){
		case SymbolEnum::INT:
		case SymbolEnum::PTR:
			return 4;
			break;

		case SymbolEnum::CHAR:
			return 1;
			break;

		case SymbolEnum::FLOAT:
			return 8;
			break;

		case SymbolEnum::ARRAY:
			return this->width * this->array_type->getSize();
			break;

		default:
			return 0;
			break;
	}
}

/*
	The function to get the string representation of the SymbolType.
	This function manually return the type of the of the Symbol defined in the SymbolType.
	For type:
		1. INT: It returns "int".
		2. PTR: It returns "ptr(" + the array type of the pointer + ")".
		3. CHAR: It returns "char".
		4. FLOAT: It returns "float".
		5. ARRAY: It returns "array(" + the width of the array + ", " + the array type of the array + ")".
*/
string SymbolType::toString(){
	switch (this->type){
		case SymbolEnum::VOID:
			return "void";
			break;

		case SymbolEnum::INT:
			return "int";
			break;

		case SymbolEnum::FLOAT:
			return "float";
			break;

		case SymbolEnum::CHAR:
			return "char";
			break;

		case SymbolEnum::PTR:
			return "ptr(" + this->array_type->toString() + ")";
			break;

		case SymbolEnum::FUNC:
			return "function";
			break;

		case SymbolEnum::ARRAY:
			return "array(" + to_string(this->width) + ", " + this->array_type->toString() + ")";
			break;

		case SymbolEnum::BLOCK:
			return "block";
			break;

		default:
			return "";
			break;
	}
}

/*
	The constructor for the SymbolTable.
*/
SymbolTable::SymbolTable(const string &name, SymbolTable *parent): name(name), parent(parent) {}

/*
	The function to lookup the symbol in the SymbolTable.
	This function checks if the symbol is present in the current table.
	If it is not present, then it checks in the parent table.
	If it is not present in the parent table, then it creates a new symbol in the current table.
*/
Symbol *SymbolTable::lookup(const string &name_)
{
	if (this->symbols.contains(name_))
	{
		return &this->symbols[name_];
	}

	Symbol *ret_ptr = nullptr;

	if (this->parent)
	{
		ret_ptr = this->parent->lookup(name_);
	}

	if (this == current_table && !ret_ptr)
	{
		this->symbols[name_] = Symbol(name_);
		return &this->symbols[name_];
	}

	return ret_ptr;
}

/*
	The function to update the SymbolTable.
	This function updates the offset of the symbols in the table.
	It also updates the offset of the symbols in the nested tables.
	How it works:
		1. It initializes the offset to 0.
		2. It iterates over all the symbols in the table.
			a) If the symbol is the first symbol in the table, then it sets the offset of the symbol to 0.
			b) Else, it sets the offset of the symbol to the offset.
			c) It updates the offset by adding the size of the symbol.
		3. It remembers the children tables.
		4. It updates the children tables.
*/
void SymbolTable::update()
{
	vector<SymbolTable *> visited{}; 
	size_t offset{};

	for (auto &&map_entry : this->symbols){
		if (map_entry.first == this->symbols.begin()->first){
			map_entry.second.offset = 0;
			offset = map_entry.second.size;
		}
		else{
			map_entry.second.offset = offset;
			offset += map_entry.second.size;
		}

		if (map_entry.second.nested_table){
			visited.push_back(map_entry.second.nested_table);
		}
	}

	for (auto &&table : visited){
		table->update();
	}
}

void SymbolTable::print()
{
	cout << string(140, '+') << '\n';
	cout << "Symbol Table Name: `" << this->name << "`\t\t Parent Table Name: `" << (this->parent ? this->parent->name : "NONE") << "`\n";
	cout << string(140, '+') << '\n';

	cout << setw(20) << "Name"<< setw(40) << "Type"<< setw(20) << "Initial Value"<< setw(20) << "Size"<< setw(20) << "Offset"<< setw(20) << "Child"<< '\n';
	cout << setw(20) << "----"<< setw(40) << "----"<< setw(20) << "-------------"<< setw(20) << "------"<< setw(20) << "----"<< setw(20) << "-----"<< "\n\n";

	vector<SymbolTable *> tovisit{};

	for (auto &&map_entry : this->symbols)
	{
		cout << setw(20) << quoted(map_entry.first, '`')
			 << setw(40) << quoted(map_entry.second.is_function ? "function" : map_entry.second.type->toString(), '`')
			 << setw(20) << (map_entry.second.initial_value == "" or map_entry.second.initial_value.empty() ? "" : '`' + map_entry.second.initial_value + '`')
			 << setw(20) << quoted(to_string(map_entry.second.size), '`')
			 << setw(20) << quoted(to_string(map_entry.second.offset), '`')
			 << setw(20) << quoted(map_entry.second.nested_table ? map_entry.second.nested_table->name : "NULL", '`')
			 << '\n';

		if (map_entry.second.nested_table)
		{
			tovisit.push_back(map_entry.second.nested_table);
		}
	}

	cout << string(140, '+') << string(4, '\n');

	for (auto &&table : tovisit)
	{
		table->print();
	}
}

/*
	The constructor for the Symbol.
*/
Symbol::Symbol(const string &name_, SymbolType::SymbolEnum type_, const string &init)
	: name(name_), offset(0), type(new SymbolType(type_)),
	  nested_table(nullptr), initial_value(init)
{
	this->size = this->type->getSize();
}

/*
	The function to update the Symbol.
	This function updates the type of the symbol.
	It also updates the size of the symbol.
*/
Symbol *Symbol::update(SymbolType *type_)
{
	this->type = type_;
	size = this->type->getSize();
	return this;
}

/*
	The function to convert the Symbol.
	This function converts the symbol to the target type.
	For type:
		1. If the current type is float:
			a) If the target type is int, then it generates a new symbol of the new type.
			b) It emits the quad for the conversion.
			c) It returns the new symbol.
		2. If the current type is int:
			a) If the target type is float, then it generates a new symbol of the new type.
			b) It emits the quad for the conversion.
			c) It returns the new symbol.
		3. If the current type is char:
			a) If the target type is int, then it generates a new symbol of the new type.
			b) It emits the quad for the conversion.
			c) It returns the new symbol.
		4. If the target type is not int, float or char, then it returns the original symbol.
*/
Symbol *Symbol::convert(SymbolType::SymbolEnum type_){
	if (this->type->type == SymbolType::SymbolEnum::FLOAT){
		if (type_ == SymbolType::SymbolEnum::INT){
			Symbol *fin_ = gentemp(type_);
			emit("=", fin_->name, "Float_To_Int(" + this->name + ")");
			return fin_;
		}
		else if (type_ == SymbolType::SymbolEnum::CHAR){
			Symbol *fin_ = gentemp(type_);
			emit("=", fin_->name, "Float_To_Char(" + this->name + ")");
			return fin_;
		}
		return this;
	}
	else if (this->type->type == SymbolType::SymbolEnum::INT)
	{
		if (type_ == SymbolType::SymbolEnum::FLOAT)
		{
			Symbol *fin_ = gentemp(type_);
			emit("=", fin_->name, "Int_To_Float(" + this->name + ")");
			return fin_;
		}
		else if (type_ == SymbolType::SymbolEnum::CHAR)
		{
			Symbol *fin_ = gentemp(type_);
			emit("=", fin_->name, "Int_To_Char(" + this->name + ")");
			return fin_;
		}
		return this;
	}
	else if (this->type->type == SymbolType::SymbolEnum::CHAR){
		if (type_ == SymbolType::SymbolEnum::INT){
			Symbol *fin_ = gentemp(type_);
			emit("=", fin_->name, "Char_To_Int(" + this->name + ")");
			return fin_;
		}
		else if (type_ == SymbolType::SymbolEnum::FLOAT)
		{
			Symbol *fin_ = gentemp(type_);
			emit("=", fin_->name, "Char_To_Float(" + this->name + ")");
			return fin_;
		}
		return this;
	}
	return this;
}


/*
	The constructor for the Quad.
	It is of two types:
		a) If the arg1 is of type int, then it generates the quad with the integer value.
		b) If the arg1 is of type string, then it generates the quad with the string value.
*/
Quad::Quad(const string &op, const string &arg1, const string &arg2, const string &result): op(op), arg1(arg1), arg2(arg2), result(result) {}
Quad::Quad(const string &op, int arg1, const string &arg2, const string &result): op(op), arg1(to_string(arg1)), arg2(arg2), result(result) {}

/*
	The function to print the Quad.
	It prints the quad in the required format.
	How it works:
		a)First it prints the binary operators.
		b)Then it prints the relational operators.
		c)Then it prints the shift operators.
		d)Then it prints the special operators.
		e)If the operator is not present in the above categories, then it prints "INVALID OPERATOR".
*/
void Quad::print()
{
	auto binary_print = [&](){
		cout << "\t" << this->result<< " = " << this->arg1<< " " << this->op<< " " << this->arg2<< '\n';
	};

	auto relation_print = [&](){
		cout << "\tif " << this->arg1<< " " << this->op<< " " << this->arg2<< " goto L" << this->result<< '\n';
	};

	auto shift_print = [&](){
		cout << "\t" << this->result<< " " << this->op[0]<< " " << this->op[1]<< this->arg1<< '\n';
	};

	auto shift_print_str = [&](const string &tp){
		cout << "\t" << this->result<< " " << tp<< " " << this->arg1<< '\n';
	};

	if (this->op == "="){
		cout << "\t" << this->result<< " = " << this->arg1<< '\n';
	}
	else if (this->op == "goto"){
		cout << "\tgoto L" << this->result<< '\n';
	}
	else if (this->op == "return"){
		cout << "\treturn " << this->result<< '\n';
	}
	else if (this->op == "call"){
		cout << "\t" << this->result<< " = call " << this->arg1<< ", " << this->arg2<< '\n';
	}
	else if (this->op == "param"){
		cout << "\tparam " << this->result<< '\n';
	}
	else if (this->op == "label"){
		cout << this->result << '\n';
	}
	else if (this->op == "=[]"){
		cout << "\t" << this->result<< " = " << this->arg1<< "[" << this->arg2<< "]\n";
	}
	else if (this->op == "[]="){
		cout << "\t" << this->result<< "[" << this->arg1<< "] = " << this->arg2<< '\n';
	}
	else if (this->op == "+" or
			 this->op == "-" or
			 this->op == "*" or
			 this->op == "/" or
			 this->op == "%" or
			 this->op == "|" or
			 this->op == "^" or
			 this->op == "&" or
			 this->op == "<<" or
			 this->op == ">>"
	){
		// if the operator is binary,again print the binary operator
		binary_print();
	}
	else if (this->op == "==" or
			 this->op == "!=" or
			 this->op == "<" or
			 this->op == ">" or
			 this->op == "<=" or
			 this->op == ">="
	){
		relation_print();
	}
	else if (this->op == "=&" or
			 this->op == "=*"
	){
		shift_print();
	}
	else if (this->op == "*="){
		cout << "\t"<< "*" << this->result<< " = " << this->arg1<< '\n';
	}
	else if (this->op == "minus"){
		shift_print_str("= minus");
	}
	else if (this->op == "~"){
		shift_print_str("= ~");
	}
	else if (this->op == "!"){
		shift_print_str("= !");
	}
	else{
		cout << this->op
			 << this->arg1
			 << this->arg2
			 << this->result
			 << '\n';

		cout << "INVALID OPERATOR\n ";
	}
}

/*
	The function to emit the quad.
	It emits the quad in the required format.
	How it works:
		a) If the arg1 is of type int, then it generates the quad with the integer value.
		b) If the arg1 is of type string, then it generates the quad with the string value.
*/
void emit(const string &op, const string &result, const string &arg1, const string &arg2)
{
	quad_array.push_back(new Quad(op, arg1, arg2, result));
}
void emit(const string &op, const string &result, int arg1, const string &arg2)
{
	quad_array.push_back(new Quad(op, arg1, arg2, result));
}

/*
	The function to backpatch the list.
	It fills out the list of the instructions, of which the address field is blank in the goto instructions.
	How it works:
		a) For all the addresses in the list, add the target address.
*/
void backpatch(const list<size_t> &list_, size_t addr)
{
	for (auto &i : list_)
	{
		quad_array[i - 1]->result = to_string(addr);
	}
}

/*
`	This make s the list for each of the statements.
*/
list<size_t> make_list(size_t base){
	return {base};
}

/*
	This merges the list of the instructions, for which two conditions are to be merged as per the Syntax Directed Translation.
*/
list<size_t> merge_list(list<size_t> &first, list<size_t> second){
	list<size_t> ret = move(first);
	ret.merge(second);
	return ret;
}

/*
	The function to get the temporary symbol for the Bool Type.
	Hoe it works:
		a) It generates the temporary symbol.
		b) It increments the temporary count.
		c) It returns the temporary symbol.
*/
void Expression::to_int()
{
	if (this->type == Expression::ExprEnum::BOOL)
	{
		this->symbol = gentemp(SymbolType::SymbolEnum::INT);
		backpatch(this->true_list, (quad_array.size() + 1));  // update the true list
		emit("=", this->symbol->name, "true");				  // emit the quad
		emit("goto", to_string(quad_array.size() + 2));		  // emit the goto quad
		backpatch(this->false_list, (quad_array.size() + 1)); // update the false list
		emit("=", this->symbol->name, "false");
	}
}

/*
	The function to get the temporary symbol,for the NonBool Type.
	Hoe it works:
		a) It generates the temporary symbol.
		b) It increments the temporary count.
		c) It returns the temporary symbol.
*/
void Expression::to_bool(){
	if (this->type == Expression::ExprEnum::NONBOOL){
		this->false_list = make_list(quad_array.size() + 1); 
		emit("==", "", this->symbol->name, "0");			 
		this->true_list = make_list(quad_array.size() + 1);	 
		emit("goto", "");
	}
}

/*
	This is the function which is used to get the next instruction.
*/
size_t next_instruction(){
	return quad_array.size() + 1;
}

/*
	This is the function which is used to get the temporary symbol.
*/
Symbol *gentemp(SymbolType::SymbolEnum type, const string &s){
	string &&name = "t" + to_string(temp_count++);
	return &(current_table->symbols[name] = Symbol(name, type, s));
}

// change the reference of the table
void change_table(SymbolTable *table){
	current_table = table;
}

/*
	This fucntion checks the type of the symbol.
	How it works:
		a) It checks if the types are same.
		b) If the types are not same but can be cast safely according the rules, then cast and return.
		c) If not possible to cast safely to the same type, then return false.
*/
bool type_check(Symbol *&a, Symbol *&b)
{
	function<bool(SymbolType *, SymbolType *)>
		type_comp = [&](SymbolType *first, SymbolType *second) -> bool
	{
		if (not first and not second){
			return true;
		}
		else if (not first or
				 not second or
				 first->type != second->type
		){
			return false;
		}
		else{
			return type_comp(first->array_type, second->array_type);
		}
	};
	if (type_comp(a->type, b->type))
		return true;
	else if (a->type->type == SymbolType::SymbolEnum::FLOAT or
			 b->type->type == SymbolType::SymbolEnum::FLOAT)
	{
		a = a->convert(SymbolType::SymbolEnum::FLOAT);
		b = b->convert(SymbolType::SymbolEnum::FLOAT);
		return true;
	}
	else if (a->type->type == SymbolType::SymbolEnum::INT or
			 b->type->type == SymbolType::SymbolEnum::INT)
	{
		a = a->convert(SymbolType::SymbolEnum::INT);
		b = b->convert(SymbolType::SymbolEnum::INT);
		return true;
	}
	else
	{
		return false;
	}
}

void yyerror(const string &s)
{
	printf("ERROR found at %d line : %s\n", yylineno, s.c_str());
}

void yyinfo(const string &s)
{
	printf("Information at %d line : %s\n", yylineno, s.c_str());
}



// TreeNode *createNode(char *name, int childCount)
// {
//    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
//    if (name != NULL)
//    {
//       newNode->nodeName = strdup(name);
//    }
//    else
//    {
//       newNode->nodeName = NULL;
//    }
//    newNode->childCount = 0;
//    newNode->maxChildren = childCount;
//    newNode->children = (TreeNode **)malloc(sizeof(TreeNode *) * childCount);
//    return newNode;
// }

// // Function to add a child to a node
// void addChild(TreeNode *parent, TreeNode *child)
// {
//    parent->children[parent->childCount++] = child;
// }

// // Function to create leaf node
// TreeNode *createLeaf(char *name)
// {
//    return createNode(name, 0);
// }

// // Function to free the memory allocated for the tree
// void freeTree(TreeNode *node)
// {
//    if (node == NULL)
//       return;

//    for (int i = 0; i < node->childCount; i++)
//    {
//       freeTree(node->children[i]);
//    }

//    free(node->children);
//    if(node->nodeName!=NULL){
//       free(node->nodeName);
//    }
//    free(node);
// }

// void printParseTree(TreeNode *node, int indent)
// {
//    if (node == NULL)
//       return;

//    // Print the current node with the given indentation
//    for (int i = 0; i < indent; i++)
//       printf("    ");
//    printf("%s\n", node->nodeName);

//    // Print all children with increased indentation
//    for (int i = 0; i < node->childCount; i++)
//    {
//       printParseTree(node->children[i], indent + 1);
//    }
// }

// int main()
// {
//    yyparse();

   // // Check if parsing was successful
   // if (root)
   // {
   //    // Print the parse tree starting from the root
   //    printf("The nodes at same indentation level are the siblings of same parent.\n");
   //    printf("Parse Tree:\n");
   //    printParseTree(root, 0);

   //    // Free the allocated memory for the parse tree
   //    // freeTree(root);
   // }
   // else
   // {
   //    printf("Parsing failed or no parse tree was generated.\n");
   // }

//    return 0;
// }



int main()
{
	// initialization of global variables
	table_count = 0;
	temp_count = 0;
	global_table = new SymbolTable("global");
	current_table = global_table;
	cout << left; 

	yyparse();

	list<size_t> l1, l2;
	l2 = merge_list(l1, make_list(next_instruction()));

	global_table->update();
	global_table->print();
	int ins{};

	for (auto &&it : quad_array)
	{
		cout << setw(6) << "L" + to_string(++ins) << ":\t";
		it->print();
	}
}

