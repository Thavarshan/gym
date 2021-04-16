/**
 * @file invoice.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundamentals Assignment).
 * @version 2.6.2
 * @date 2021-04-16
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "include/invoice.h"
#include "include/details.h"
#include "include/fort.hpp"
#include "include/menus.h"
#include "include/utils.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

/**
 * @brief Display the invoice of the purchase details for the user.
 *
 * @param details
 */
void generateInvoice(std::map<std::string, double> &details)
{
      // Before we generate an invoice we need to ask for the name of the user/customer.
      std::string name = askName(); // This function is found in "utils.cpp"

      // We also need to generate a unique number for the invoice.
      std::string invoiceTitle = "Invoice No " + std::to_string(std::rand());

      // We then use the table creator module from 'libfort' library to generate
      // a table to contain all the relevant content to be displayed.
      fort::char_table table;

      // We set a table border style.
      table.set_border_style(FT_BASIC2_STYLE);

      // Now we fill the table with content.

      // Row 1 content goes here.
      table << invoiceTitle
            << "RATHNAYAKA GYMS\n"
               "Beyond Fitness"
            << fort::endr;

      // Row 2
      table << "Customer Name" << name << fort::endr;

      // Row 3
      table << "Date" << currentDateTime() << fort::endr;

      // Row 4
      table << "Service Name"
            << "Price (Rs)"
            << "Units"
            << "Value" << fort::endr;

      // Row 5 and 6 contain information about which packages the user purchased and how many of each they purchased.
      for (auto const &[id, units] : details)
      {
            // First we determine if the given string is a valid package name.
            if (isPackage(id)) // This function is found in "details.cpp"
            {
                  // If it is we gather information on it.
                  double price = packagePriceLookup(id); // This function is found in "details.cpp"

                  // And print it out to display the user's purchases.
                  table << packageNameLookup(id) // This function is found in "details.cpp"
                        << price
                        << units
                        << price * units << fort::endr;
            }
      }

      // Row 7
      table << "Total Amount"
            << ""
            << ""
            << details["total"] << fort::endr;

      // Row 8
      table << "Discount Earned"
            << ""
            << ""
            << details["discount"] << fort::endr;

      // Row 9
      table << "Total Due Amount"
            << ""
            << ""
            << details["due"] << fort::endr;

      // Table formatting to give an invoice specific design.
      table[0][0].set_cell_span(2);
      table[0][1].set_cell_span(3);

      table[1][0].set_cell_span(2);
      table[1][1].set_cell_span(3);

      table[2][0].set_cell_span(2);
      table[2][1].set_cell_span(3);

      table[6][0].set_cell_span(3);
      table[7][0].set_cell_span(3);
      table[8][0].set_cell_span(3);

      // We need to convert the table content to string format.
      std::string invoice = table.to_string();

      // Here we write the table contents to a ".txt" file.
      writeFile("./invoices/" + invoiceTitle + ".txt", invoice); // This function is found in "utils.cpp"

      // We finally print the table contents to the screen with a thank you message.
      printf("\n");
      printf("Thank you for your purchase!\n");
      printf("This is your receipt\n");
      printf("\n");
      printf("%s\n", invoice.c_str());
      printf("\n");
}
