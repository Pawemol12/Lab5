// @author: Paweł Lodzik
// @date 23.01.2018r 01:15 am
// @description Zadanie 5 

#include "stdafx.h"
#include <iostream>
#include <string>
#include <map>

using namespace std;

class Serwer;
class Klient;
class ExtKlient;
class ExtSerwer;

class Klient {
protected:
	Serwer *s;
	string nazwa;

public:
	Klient(const string nazwa, Serwer *s) : nazwa(nazwa), s(s) {

	}

	Klient(const Klient &client);

	Klient & operator =(const Klient &client);
	void send(string msg);

	string getNazwa() { return nazwa; };
};

class ExtKlient : public Klient {
	private:
		int messageCount;
	public:
		ExtKlient(const string nazwa, Serwer *s) : Klient(nazwa, s), messageCount(0) {

		};

		void send(string msg);
		int getMessageCount() { return this->messageCount; };
		~ExtKlient();
};

struct Dane {
	Klient *cli; //adres klienta, który wysłał wiadomość
	string msg; //odebrana wiadomość
	Dane() {};
	Dane(Klient *cli, string msg) : cli(cli), msg(msg) {

	};
};

class Serwer {
	protected:
		map < Klient *, Dane > dane_klientow;

	public:
		void receive(string msg, Klient *cli);
		void drukuj();
		virtual void remove(Klient *cli) {};
		~Serwer();
};

class ExtSerwer : public Serwer {
	public:
		void remove(Klient *cli);
};

Klient::Klient(const Klient &client) {
	if (this == &client) { //Zabezpiecznie przed kopiowaniem samego siebie
		return;
	}


	this->s = client.s;
	this->nazwa = client.nazwa;
}

Klient & Klient::operator=(const Klient &client) {
	if (this == &client) { //Zabezpiecznie przed kopiowaniem samego siebie
		return *this;
	}
	this->s = client.s;
	this->nazwa = client.nazwa;
	return *this;
}

void Klient::send(string msg) {
	this->s->receive(msg, this);
}

void Serwer::receive(string msg, Klient *cli) {
	auto it = dane_klientow.find(cli);
	
	//Jeśli klient znajduje się już w mapie, dopisujemy do wiadomości po prostu tekst
	if (it != dane_klientow.end()) {
		dane_klientow[cli].msg += "#" + msg;
		return;
	}

	//W przeciwnym wypadku dodajemy go do mapy
	dane_klientow[cli] = Dane(cli, msg);
}

void Serwer::drukuj() {
	int itNr = 0;
	cout << "Nr.\tAdres Klienta\tWiadomosc" << endl;
	for (auto &it : dane_klientow) {
		itNr++;
		cout << itNr << ".\t" << it.first->getNazwa() << "\t" << it.second.msg << endl;
	}
}

Serwer::~Serwer() {
	for (auto &it : dane_klientow) {
		delete it.first;
	}
	dane_klientow.clear();
}

void ExtSerwer::remove(Klient *cli) {
	
	//Znajdujemy adres klienta
	auto it = dane_klientow.find(cli);

	if (it != dane_klientow.end()) {
		dane_klientow.erase(cli);
		delete cli;
	}

}

void ExtKlient::send(string msg) {
	Klient::send(msg);
	messageCount++;
}

ExtKlient::~ExtKlient() {
	this->s->remove(this);
};

int main()
{
	Serwer serwer1;

	Klient *klient1 = new Klient("Pawel", &serwer1);
	Klient *klient2 = new Klient("Daniel", &serwer1);

	klient1->send("Cos tam");
	klient1->send("Cos tam2");
	klient1->send("Cos tam3");

	klient2->send("HAHAHA");
	klient2->send("Xd");
	klient2->send("LOL");

	cout << "Zawartość słownika serwer 1" << endl;
	serwer1.drukuj();

	ExtSerwer serwer2;
	ExtKlient *extKlient1 = new ExtKlient("Damian", &serwer2);
	ExtKlient *extKlient2 = new ExtKlient("Daniel", &serwer2);
	ExtKlient *extKlient3 = new ExtKlient("Pawemol", &serwer2);

	extKlient1->send("A daj se spokoj");
	extKlient1->send("Nie wiem");
	extKlient1->send("Albo Wiem");

	extKlient2->send("A kaj tam xd");
	extKlient2->send("Nic tu nie ma");
	extKlient2->send("A moze jest");

	extKlient3->send("NCS");
	extKlient3->send("is the best");
	cout << "Zawartość słownika serwer 2" << endl;
	serwer2.drukuj();
	system("pause");
    return 0;
}

