#ifndef CMERRENYITOTT_H_INCLUDED
#define CMERRENYITOTT_H_INCLUDED

#include "../cimke.h"

struct CmerreNyitott : public Cimke{


    CmerreNyitott(){IDname="merreNyit"; comperator=true;}

	CmerreNyitott* clone() const override { return new CmerreNyitott(*this); }

    int getI() override;
	bool readIn(ifstream ifs) override;
	bool writeOut(ofstream ofs) override;

    bool check(Stock& stock, Datum datum, Datum datumhoz) override;
    float getValue(Stock& stock, Datum datum) override;
	float getDiffValue(Stock& stock, Datum from, Datum to) override;

    bool check(Stock& stock, Negyed datum, Negyed datumhoz);
    float getValue(Stock& stock, Negyed datum) override;
    float getDiffValue(Stock& stock, Negyed from, Negyed to) override;
};


#endif // CMERRENYITOTT_H_INCLUDED
