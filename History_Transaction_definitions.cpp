#ifndef _HISTORY_TRANSACTION_CPP_
#define _HISTORY_TRANSACTION_CPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "project4.hpp"
#include "History.hpp"
#include "Transaction.hpp"

////////////////////////////////////////////////////////////////////////////////
// Definitions for Transaction class
////////////////////////////////////////////////////////////////////////////////
//
//

// Constructor
// TASK 1
//
Transaction::Transaction( std::string ticker_symbol,  unsigned int day_date,
		unsigned int month_date,  unsigned year_date,
		bool buy_sell_trans,  unsigned int number_shares,
		double trans_amount )
{
	symbol = ticker_symbol;
	day = day_date;
	month = month_date;
	year = year_date;
	shares = number_shares; //Number of shares after the transaction completes
	amount = trans_amount; //Money paid/earned

	if (buy_sell_trans == true){ //If it's buying
		trans_type = "Buy";
	}else{
		trans_type = "Sell";
	}

	trans_id = assigned_trans_id;
	assigned_trans_id++;

	acb = 0;
	acb_per_share = 0;
	share_balance = 0;
	cgl = 0;

	//stores the address of the next Transaction in the linked list
	//Transaction *p_list_head{nullptr}; //Head of the linked list
	//p_next = p_list_head;

	p_next = nullptr;

}


// Destructor
// TASK 1
//
Transaction::~Transaction(){
	//Transaction *p_tr{p_head};

	delete p_next;
	p_next = nullptr;
}


// Overloaded < operator.
// TASK 2
//
bool Transaction::operator<( Transaction const &other ){

	if (this->year < other.year){ //When the first trade year is before the second one
		return true;
	}else if (this->year == other.year && this->month < other.month){ //whether first trade month is before the second one
		return true;
	}else if (this->year == other.year && this->month == other.month && this->day < other.day){ //whether first trade date is before the second one
		return true;
	}
	return false;
}


// GIVEN
// Member functions to get values.
//
std::string Transaction::get_symbol() const { return symbol; }
unsigned int Transaction::get_day() const { return day; }
unsigned int Transaction::get_month() const { return month; }
unsigned int Transaction::get_year() const { return year; }
unsigned int Transaction::get_shares() const { return shares; }
double Transaction::get_amount() const { return amount; }
double Transaction::get_acb() const { return acb; }
double Transaction::get_acb_per_share() const { return acb_per_share; }
unsigned int Transaction::get_share_balance() const { return share_balance; }
double Transaction::get_cgl() const { return cgl; }
bool Transaction::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Transaction::get_trans_id() const { return trans_id; }
Transaction *Transaction::get_next() { return p_next; }

// GIVEN
// Member functions to set values.
//
void Transaction::set_acb( double acb_value ) { acb = acb_value; }
void Transaction::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Transaction::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Transaction::set_cgl( double value ) { cgl = value; }
void Transaction::set_next( Transaction *p_new_next ) { p_next = p_new_next; }

// GIVEN
// Print the transaction.
//
void Transaction::print() {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << std::setw(4) << get_trans_id() << " "
			<< std::setw(4) << get_symbol() << " "
			<< std::setw(4) << get_day() << " "
			<< std::setw(4) << get_month() << " "
			<< std::setw(4) << get_year() << " ";


	if ( get_trans_type() ) {
		std::cout << "  Buy  ";
	} else { std::cout << "  Sell "; }

	std::cout << std::setw(4) << get_shares() << " "
			<< std::setw(10) << get_amount() << " "
			<< std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
			<< std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
			<< std::setw(10) << std::setprecision(3) << get_cgl()
			<< std::endl;
}


////////////////////////////////////////////////////////////////////////////////
// Definitions for the History class
////////////////////////////////////////////////////////////////////////////////
//
//


// Constructor
// TASK 3
//

History::History(){
	p_head = nullptr;

}

// Destructor
// TASK 3
//
//History::~History(){
//
//	if (p_head->get_next() == nullptr){ //if there is only one node in the list
//		delete p_head;
//		p_head = nullptr;
//	}else{
//		while (p_head != nullptr){ //Traverse through the list
//			Transaction *p_temp{p_head};
//
//			delete p_temp;
//			p_temp = nullptr;
//
//			p_head = p_head->get_next();
//		}
//	}
//}

History::~History(){
	//Transaction *p_temp{p_head};

	while (p_head != nullptr){
		Transaction *p_temp{p_head};

		delete p_temp;
		p_temp = nullptr;

		p_head = p_head->get_next();
	}
}

// read_transaction(...): Read the transaction history from file.
// TASK 4
//
void History::read_history(){
	ece150::open_file();

	while (ece150::next_trans_entry() == true){
		Transaction *p_node = new Transaction(ece150::get_trans_symbol(), ece150::get_trans_day(), ece150::get_trans_month(), ece150::get_trans_year(),  ece150::get_trans_type(), ece150::get_trans_shares(), ece150::get_trans_amount());
		insert(p_node);
	}

	ece150::close_file();
}

// insert(...): Insert transaction into linked list.
// TASK 5
//
void History::insert(Transaction *p_new_trans){
	Transaction *p_new_node{p_head};

	if (p_head == nullptr){ //While the linked list is empty
		p_head = p_new_trans; //Just insert the transaction
	}else{
		while (p_new_node->get_next() != nullptr){ //Tranverse til the last node
			p_new_node = p_new_node->get_next(); //p_node is now at the last node
		}
		p_new_node->set_next(p_new_trans); //set p_node to the transaction passed in
	}
}


// sort_by_date(): Sort the linked list by trade date.
// TASK 6
//
//void History::sort_by_date(){
//	Transaction *p_sorted{nullptr};
//	Transaction *p_temp2{nullptr};
//
//	if (p_head == nullptr || p_head->get_next() == nullptr){
//		return;
//	}else{
//		while (p_head != nullptr){ //while the linked list is not empty
//
//			Transaction *p_temp1{p_head};
//			p_head = p_head->get_next();
//			p_sorted = p_temp1;
//			p_temp1->set_next(nullptr);
//
//			if (*p_temp1 < *p_sorted){
//				p_temp1->set_next(p_sorted);
//				p_sorted = p_sorted->get_next();
//			}else{
//				p_temp2 = p_sorted;
//				//While next node in the list is bigger and we are not hitting the end of the list
//				while (!(*p_temp1 < *(p_temp2->get_next()) && p_temp2->get_next() != nullptr)){
//					p_temp2 = p_temp2->get_next();
//				}
//				//insert the node
//				p_temp1->set_next(p_temp2->get_next());
//				p_temp2->set_next(p_temp1);
//			}
//		}
//		p_head = p_sorted;
//	}

void History::sort_by_date(){
	Transaction *p_sorted_head{nullptr};
	Transaction *p_sorted_curr{nullptr};

	if (p_head == nullptr || p_head->get_next() == nullptr)
	{
		return;
	}

	// 2 4 3 5 7
	Transaction *p_unsorted_curr{p_head};
	p_head = p_head->get_next();
	p_sorted_head = p_unsorted_curr;  // 2 -> 4, 4 3 5 7
	p_sorted_head->set_next(nullptr); // 2 -> null, 4 3 5 7

	// std::cout << "p_sorted_head \n";
	// p_sorted_head->print();
	// std::cout << "p_head \n";
	// p_head->print();

	while (p_head != nullptr)
	{
		p_unsorted_curr = p_head;
		p_head = p_head->get_next();

		if (*p_unsorted_curr < *p_sorted_head) // curr smaller than all sorted values
		{
			p_unsorted_curr->set_next(p_sorted_head);
			p_sorted_head = p_unsorted_curr;
		}
		else
		{
			p_sorted_curr = p_sorted_head;
			while (p_sorted_curr->get_next() != nullptr && *p_sorted_curr->get_next() < *p_unsorted_curr)
			{
				// p_sorted_curr->print();
				p_sorted_curr = p_sorted_curr->get_next();
			}
			if (p_sorted_curr->get_next() == nullptr) { // larger than anything already in sorted list
				p_sorted_curr->set_next(p_unsorted_curr);
				p_unsorted_curr->set_next(nullptr);  // 1 3 5 6, 6 4
				// std::cout << "nullptr case";
				// p_sorted_curr->print();
				// p_sorted_curr->get_next()->print();
			} else { // somewhere in middle of sorted list
				p_unsorted_curr->set_next(p_sorted_curr->get_next());
				p_sorted_curr->set_next(p_unsorted_curr);  // 1 3 5, 4 2972 2

				// std::cout << "other case";
				// p_sorted_curr->print();
				// p_sorted_curr->get_next()->print();
				// p_sorted_curr->get_next()->get_next()->print();

			}
		}
	}
	p_head = p_sorted_head;
}

// update_acb_cgl(): Updates the ACB and CGL values.
// TASK 7
//
void History::update_acb_cgl(){
	Transaction *p_trans{p_head};

	if (p_trans == nullptr){ //If there is nothing in the list, just return
		return;
	}

	//If the list is not empty
	double acb = 0;
	double acb_per_share = 0;
	unsigned int share_balance = 0;
	double cgl = 0;

	// std::cout << "acb is: " << acb << std::endl;
	// std::cout << "acb_per_share is: " << acb_per_share << std::endl;
	// std::cout << "share_balance: " << share_balance << std::endl;
	// std::cout << "cgl is: " << cgl << std::endl;

	while (p_trans != nullptr){ //while the list is not empty
		if (p_trans->get_trans_type() == true)
		{ //If it's buying
			//acb
			acb = acb + p_trans->get_amount(); //acb gets updated with the current transaction's acb
			p_trans->set_acb(acb); //acb gets updated for the current transaction

			//share balance
			share_balance = share_balance + p_trans->get_shares();
			p_trans->set_share_balance(share_balance);

			//acb_per_share
			acb_per_share = acb / p_trans->get_share_balance();
			p_trans->set_acb_per_share(acb_per_share);

			//cgl
			cgl = 0;
			p_trans->set_cgl(cgl);
		}
		else //else if it's selling
		{
			//acb
			acb = acb - (p_trans->get_shares() * acb_per_share); //acb gets updated with the current transaction's acb
			p_trans->set_acb(acb); //acb gets updated for the current transaction

			//share balance
			share_balance = share_balance - p_trans->get_shares();
			p_trans->set_share_balance(share_balance);

			//acb_per_share
			acb_per_share = acb / p_trans->get_share_balance();
			p_trans->set_acb_per_share(acb_per_share);

			//cgl
			cgl = p_trans->get_amount() - (p_trans->get_shares() * p_trans->get_acb_per_share());
			p_trans->set_cgl(cgl);
		}
		p_trans = p_trans->get_next();
	}
}

// compute_cgl(): )Compute the ACB, and CGL.
// TASK 8

double History::compute_cgl(unsigned int year){
	Transaction *p_tra = p_head;
	double cgl{};

	while (p_tra != nullptr){
		if (p_tra->get_year() == year && p_tra->get_trans_type() == false){
			cgl = cgl + p_tra->get_cgl();
		}
		p_tra = p_tra->get_next();
	}
	return cgl;
}

// print() Print the transaction history.
//TASK 9
//
void History::print(){
	std::cout<<"========== BEGIN TRANSACTION HISTORY ============"<<std::endl;
	Transaction *p_print = p_head;
	while(p_print != nullptr) {
		p_print->print();
		p_print = p_print->get_next();
	}
	std::cout<<"========== END TRANSACTION HISTORY ============"<<std::endl;
}


// GIVEN
// get_p_head(): Full access to the linked list.
//
Transaction *History::get_p_head() { return p_head; }


#endif
