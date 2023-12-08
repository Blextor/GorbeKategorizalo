#ifndef CMERRENYITOTT_H_INCLUDED
#define CMERRENYITOTT_H_INCLUDED

#include "../cimke.h"

struct CmerreNyitott : public Cimke{


    CmerreNyitott(){name="merreNyitott"; comperator=true;}


    int getI() override;
	bool readIn(ifstream ifs) override;
	bool writeOut(ofstream ofs) override;

    int check(Stock* stock, Datum datum) override;
    float getValue(Stock* stock, Datum datum) override;
	float getDiffValue(Stock* stock, Datum from, Datum to) override;

    int check(Stock* stock, Negyed datum) override;
    float getValue(Stock* stock, Negyed datum) override;
    float getDiffValue(Stock* stock, Negyed from, Negyed to) override;
};


#endif // CMERRENYITOTT_H_INCLUDED
