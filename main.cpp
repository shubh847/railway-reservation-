#include "rsrc.h"
user a, usr;
list<string> stations;
list<reservation> reservations;

void get() {
	ifstream in;
	string u, p;
	int l;
	char tmp;
	in.open("user.dat", ios::binary);
	list<string>::iterator i;
	if (in.is_open()) {
		in.read((char*)&l, sizeof(int));
		u.clear();
		for (int i = 0; i < l; i++) {
			in.read((char*)&tmp, 1);
			u += tmp;
		}
		a.name = u;
		in.read((char*)&l, sizeof(int));
		p.clear();
		for (int i = 0; i < l; i++) {
			in.read((char*)&tmp, 1);
			p += tmp;
		}
		a.psw = p;
		in.read((char*)&l, sizeof(int));
		u.clear();
		for (int i = 0; i < l; i++) {
			in.read((char*)&tmp, 1);
			u += tmp;
		}
		usr.name = u;
		in.read((char*)&l, sizeof(int));
		p.clear();
		for (int i = 0; i < l; i++) {
			in.read((char*)&tmp, 1);
			p += tmp;
		}
		usr.psw = p;
	}
	in.close();
	in.open("adm.bin", ios::binary);
	if (in.is_open()) {
		in.read((char*)&l, sizeof(int));
		reservation::set_base_fare(l);
		int s;
		in.read((char*)&s, sizeof(int));
		for (int k = 0; k < s; k++) {
			in.read((char*)&l, sizeof(int));
			string s;
			if (l != 0)
				for (int j = 0; j < l; j++) {
					in.read((char*)&tmp, 1);
					s += tmp;
				}
			stations.push_back(normalizeStationName(s));
		}
		in.close();
		stations.unique();
		stations.sort();
	}
	else {
		reservation::set_base_fare(100);
		// Add default stations
		stations.push_back("mumbai");
		stations.push_back("delhi");
		stations.push_back("new delhi");
		stations.push_back("kolkata");
		stations.push_back("chennai");
		stations.push_back("bengaluru");
		stations.push_back("hyderabad");
		stations.push_back("ahmedabad");
		stations.push_back("lucknow");
		stations.push_back("dehradun");
		stations.unique();
		stations.sort();
	}
	in.close();
	in.open("resrv.txt");
	if (in.is_open()) {
		while (!in.eof()) {
			reservation r;
			in.read((char*)&r.fare, sizeof(int));
			in >> r.ticket_no;
			if(!r.ticket_no.empty())r.clas = r.ticket_no.at(r.ticket_no.length() - 1);
			getline(in, u);
			getline(in, r.source);
			getline(in, r.destination);
			r.source = normalizeStationName(r.source);
			r.destination = normalizeStationName(r.destination);
			getline(in, r.reserver.name);
			in >> r.reserver.aadhaar;
			in >> r.reserver.number;
			getline(in, u);
			getline(in, r.reserver.address);
			in >> r.arrival_date.day;
			in >> tmp;
			in >> r.arrival_date.month;
			in >> tmp;
			in >> r.arrival_date.year;
			getline(in, u);
			getline(in, u);
			reservations.push_back(r);
		}
		reservations.pop_back();
	}
	else reservation::set_base_fare(100);
}

void put() {
	char tmp;
	list<string>::iterator i;
	int l;
	ofstream out("adm.bin", ios::binary);
	l = reservation::get_base_fare();
	out.write((char*)&l, sizeof(int));
	int s = stations.size();
	out.write((char*)&s, sizeof(int));
	for (i = stations.begin(); i != stations.end(); i++) {
		l = (*i).length();
		if (l != 0) out.write((char*)&l, sizeof(int));
		for (int j = 0; j < l; j++) {
			tmp = (*i).at(j);
			out.write((char*)&tmp, 1);
		}
	}
	out.close();
	out.open("resrv.txt");
	list<reservation>::iterator j;
	for (j = reservations.begin(); j != reservations.end(); j++) {
		(*j).put(out);
	}
	out.close();
}

void displayStations() {
	stations.unique();
	stations.sort();
	cout << "Available stations:" << endl;
	if (stations.empty()) {
		cout << "\tNO RECORDS FOUND" << endl;
		return;
	}
	int n = 0;
	for (auto &st : stations) {
		cout << ++n << ". " << st << endl;
	}
}

void clearScreen() {
	system("CLS");
}

void printBorder() {
	cout << "╔══════════════════════════════════════════════╗" << endl;
}

void printBorderBottom() {
	cout << "╚══════════════════════════════════════════════╝" << endl;
}

void printHeader(const string& title) {
	clearScreen();
	printBorder();
	cout << "║  🚂  RAILWAY RESERVATION SYSTEM  🚂          ║" << endl;
	cout << "╠══════════════════════════════════════════════╣" << endl;
	cout << "║  " << title;
	for (int i = 0; i < (44 - 2 - title.length()); i++) cout << " ";
	cout << "║" << endl;
	cout << "╠══════════════════════════════════════════════╣" << endl;
}

void printMenuLine(char opt, const string& desc) {
	cout << "║  [" << opt << "]  " << desc;
	for (int i = 0; i < (39 - desc.length()); i++) cout << " ";
	cout << "║" << endl;
}

void printEmpty() {
	cout << "║" << string(44, ' ') << "║" << endl;
}

void admin() {
	ifstream in;
	string u, p;
	int l, n;
	char tmp;
	
	list<string>::iterator i;
	list<reservation>::iterator r;
	do {
		printHeader("ADMIN PANEL");
		printMenuLine('0', "Log Out");
		printMenuLine('1', "Add New Station");
		printMenuLine('2', "Delete Station");
		printMenuLine('3', "Display All Stations");
		printMenuLine('4', "Display All Reservations");
		printMenuLine('5', "Update Base Fares");
		printEmpty();
		cout << "║  Enter choice: ";
		do {
			tmp = _getch();
		} while (tmp<'0' || tmp>'5');
		cout << tmp << string(27, ' ') << "║" << endl;
		printBorderBottom();
		string s;
		switch (tmp) {
		case '0':
			break;
		case '1': {
			printHeader("ADD NEW STATION");
			cout << "║  Station name: ";
			string station;
			cin.ignore();
			do {
				getline(cin, s);
			} while (s.empty());
			stations.push_back(normalizeStationName(s));
			cout << "║  " << string(42, ' ') << "║" << endl;
			cout << "║  ✓ Station added successfully!           ║" << endl;
			printBorderBottom();
			Sleep(1500);
			break;
		}
		case '2': {
			printHeader("DELETE STATION");
			cout << "║  Station name: ";
			cin.ignore();
			do {
				getline(cin, s);
			} while (s.empty());
			s = normalizeStationName(s);
			stations.remove(s);
			cout << "║  ✓ Station deleted successfully!          ║" << endl;
			printBorderBottom();
			Sleep(1500);
			break;
		}
		case '3': {
			stations.unique();
			stations.sort();
			printHeader("AVAILABLE STATIONS");
			if (stations.empty()) {
				cout << "║  NO RECORDS FOUND" << string(26, ' ') << "║" << endl;
				printBorderBottom();
				Sleep(2000);
				break;
			}
			n = 0;
			for (i = stations.begin(); i != stations.end(); i++) {
				n++;
				string line = "  " + to_string(n) + ". " + (*i);
				cout << "║  " << line;
				for (int j = 0; j < (42 - line.length()); j++) cout << " ";
				cout << "║" << endl;
				
				if (n % 15 == 0) {
					cout << "║  Press ESC to exit, any other key to cont ║" << endl;
					printBorderBottom();
					tmp = _getch();
					if (tmp == 27) break;
					printHeader("AVAILABLE STATIONS");
				}
			}
			printBorderBottom();
			Sleep(1500);
			break;
		}
		case '4': {
			n = 0;
			if (reservations.empty()) {
				printHeader("ALL RESERVATIONS");
				cout << "║  NO RESERVATIONS FOUND" << string(20, ' ') << "║" << endl;
				printBorderBottom();
				Sleep(2000);
			} else {
				for (r = reservations.begin(); r != reservations.end(); r++) {
					clearScreen();
					printBorder();
					cout << "║  ✓ RESERVATION #" << ++n << " of " << reservations.size() << string(25, ' ') << "║" << endl;
					cout << "╠══════════════════════════════════════════════╣" << endl;
					cout << (*r);
					cout << "║  Press ESC to exit, any other key for next   ║" << endl;
					printBorderBottom();
					tmp = _getch();
					if (tmp == 27) break;
				}
			}
			break;
		}
		case '5': {
			printHeader("UPDATE BASE FARE");
			cout << "║  New base fare (₹): ";
			cin >> n;
			reservation::set_base_fare(n);
			cout << "║  ✓ Fare updated to ₹" << n << string(18, ' ') << "║" << endl;
			printBorderBottom();
			Sleep(1500);
			break;
		}
		}
		if (tmp != '0') {
			put();
		}
	} while (tmp != '0');
	put();
}

void dat_entr() {
	char tmp;
	string s, t;
	reservation r;
	list<reservation>::iterator ri;
	do {
		printHeader("USER DASHBOARD");
		printMenuLine('0', "Log Out");
		printMenuLine('1', "Reserve Your Seat");
		printMenuLine('2', "Cancel Reservation");
		printEmpty();
		cout << "║  Enter choice: ";
		do {
			tmp = _getch();
		} while (tmp<'0' || tmp>'2');
		cout << tmp << string(27, ' ') << "║" << endl;
		printBorderBottom();
		switch (tmp) {
		case '0':
			break;
		case '1': {
			printHeader("NEW RESERVATION");
			r.in(stations);
			reservations.push_back(r);
			cout << "║  ✓ You have been reserved successfully!    ║" << endl;
			cout << "║  Please note your ticket number above       ║" << endl;
			printBorderBottom();
			Sleep(2000);
			break;
		}
		case '2': {
			printHeader("CANCEL RESERVATION");
			cout << "║  Ticket No: ";
			cin >> s;
			bool f = false;
			for (ri = reservations.begin(); ri != reservations.end(); ri++) {
				t = (*ri).ticket();
				if (t == s) {
					f = true;
					r = *ri;
					clearScreen();
					printBorder();
					cout << "║  RESERVATION DETAILS" << string(23, ' ') << "║" << endl;
					cout << "╠══════════════════════════════════════════════╣" << endl;
					cout << r;
					cout << "║  Confirm cancellation? (Y/N): ";
					do {
						tmp = _getch();
					} while (tmp != 'Y' && tmp != 'N' && tmp != 'y' && tmp != 'n');
					if (tmp == 'Y' || tmp == 'y') {
						cout << "Y" << string(12, ' ') << "║" << endl;
						cout << "║  ✓ Reservation cancelled successfully!      ║" << endl;
						reservations.remove(r);
					}
					else {
						cout << "N" << string(12, ' ') << "║" << endl;
						cout << "║  Cancellation aborted!                      ║" << endl;
					}
					printBorderBottom();
					Sleep(1500);
					break;
				}
			}
			if (!f) {
				cout << "║  ✗ No reservation found with this number!  ║" << endl;
				printBorderBottom();
				Sleep(1500);
			}
			break;
		}
		}
		put();
	} while (tmp != '0');
}

int main() {
	system("COLOR 0F");
	get();
	char tmp = '1';
	do {
		printHeader("WELCOME TO RAILWAY RESERVATION");
		printMenuLine('0', "Exit");
		printMenuLine('1', "Log In");
		printEmpty();
		cout << "║  Enter choice: ";
		do {
			tmp = _getch();
		} while (tmp > '1' || tmp < '0');
		cout << tmp << string(27, ' ') << "║" << endl;
		printBorderBottom();
		
		if (tmp == '0') {
			cout << endl << "Thank you for using Railway Reservation System!" << endl;
			cout << "Have a great journey! 🚆" << endl << endl;
			return 0;
		}
		
		user tp;
		if (a.name.empty() && a.psw.empty()) {
			printHeader("FIRST TIME SETUP");
			cout << "║  No user accounts found.                     ║" << endl;
			cout << "║  Let's create your accounts now!             ║" << endl;
			printMenuLine(' ', "");
			ofstream out("user.dat", ios::binary);
			
			cout << "║  " << string(42, ' ') << "║" << endl;
			cout << "║  ADMIN CREDENTIALS" << string(25, ' ') << "║" << endl;
			cout << "║  " << string(42, ' ') << "║" << endl;
			cin >> tp;
			out << tp;
			a = tp;
			
			cout << "║  " << string(42, ' ') << "║" << endl;
			cout << "║  USER CREDENTIALS" << string(25, ' ') << "║" << endl;
			cout << "║  " << string(42, ' ') << "║" << endl;
			cin >> tp;
			out << tp;
			usr = tp;
			out.close();
			
			printBorderBottom();
			Sleep(1500);
			continue;
		}
		
		do {
			printHeader("LOGIN");
			cin >> tp;
			if ((tp.name != a.name || tp.psw != a.psw) && (tp.name != usr.name || tp.psw != usr.psw)) {
				printBorderBottom();
				cout << "\n❌ Invalid credentials! Try again..." << endl << endl;
				Sleep(1500);
			}
		} while ((tp.name != a.name || tp.psw != a.psw) && (tp.name != usr.name || tp.psw != usr.psw));

		if (tp.name == a.name) {
			admin();
		}
		else if (tp.name == usr.name) {
			dat_entr();
		}
		put();
	} while (tmp != '0');
	return 0;
}


int copy() {
	ifstream in("Railway Stations.txt", ios::in);
	ofstream out("adm.bin", ios::binary);
	int l = 0, n = 0;
	out.write((char*)&l, sizeof(int));
	string s;
	while (!in.eof()) {
		getline(in, s);
		n++;
		l = s.size();
		if(l != 0)out.write((char*)&l, sizeof(int));
		char tmp;
		for (int i = 0; i < l; i++) {
			tmp = s.at(i);
			out.write((char*)&tmp, 1);
		}
	}
	in.close();

	out.seekp(out.beg);
	out.write((char*)&n, sizeof(int));
	out.close();
	return 0;
}
