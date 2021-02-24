#include "fort.hpp"
#include <iostream>
#include <stdio.h>
#include <string>

void generateInvoice()
{
    std::string invoiceTitle = "Invoice No ";
    std::string invoiceNumber = "01";

    fort::char_table table;
    /* Set table border style */
    table.set_border_style(FT_DOUBLE2_STYLE);

    // Fill table with data

    // Row 1
    table << invoiceTitle + invoiceNumber
          << "RATHNAYAKA GYMS\n"
             "Beyond Fitness"
          << fort::endr;

    // Row 2
    table << "Customer Name"
          << "Curabitur" << fort::endr;

    // Row 3
    table << "Date"
          << "2018-06-07" << fort::endr;

    // Row 4
    table << "Service Name"
          << "Price (Rs)"
          << "Units"
          << "Value" << fort::endr;

    // Row 5
    table << "EXECUTIVE MEM(FULL)"
          << "5500.00"
          << "1"
          << "5500.00" << fort::endr;

    // Row 6
    table << "ON FISH OIL (100 SOFT-GELS)"
          << "3500.00"
          << "1"
          << "3500.00" << fort::endr;

    // Row 7
    table << "Total Amount"
          << ""
          << ""
          << "9000.00" << fort::endr;

    // Row 8
    table << "Discount Earned"
          << ""
          << ""
          << "450.00" << fort::endr;

    // Row 9
    table << "Total Due Amount"
          << ""
          << ""
          << "8550.00" << fort::endr;

    table[0][0].set_cell_span(2);
    table[0][1].set_cell_span(3);

    table[1][0].set_cell_span(2);
    table[1][1].set_cell_span(3);

    table[2][0].set_cell_span(2);
    table[2][1].set_cell_span(3);

    table[6][0].set_cell_span(3);

    table[7][0].set_cell_span(3);

    table[8][0].set_cell_span(3);

    std::cout << table.to_string() << std::endl;
}
