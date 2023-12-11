#ifndef CMERREZART_H_INCLUDED
#define CMERREZART_H_INCLUDED

#include "../cimke.h"

struct CmerreZart : public Cimke{



    CmerreZart(){IDname="merreZar"; comperator=false;}

	CmerreZart* clone() const override { return new CmerreZart(*this); }

    int getI() override;
	bool readIn(ifstream ifs) override;
	bool writeOut(ofstream ofs) override;
	bool readIn(vector<string> param) override;
	bool writeOut() override;

    int check(Stock* stock, Datum datum) override;
    float getValue(Stock* stock, Datum datum) override;
	float getDiffValue(Stock* stock, Datum from, Datum to) override;

    int check(Stock* stock, Negyed datum) override;
    float getValue(Stock* stock, Negyed datum) override;
    float getDiffValue(Stock* stock, Negyed from, Negyed to) override;
};


#endif // CMERREZART_H_INCLUDED
