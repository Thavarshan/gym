/**
 * @file invoice.cpp
 * @author Thavarshan Thayananthajothy (tjthavarshan@gmail.com) <CL/HDCSE/95/15>
 * @brief Rathnayaka GYMS Application (ICBT Batch 95 - Programming Fundementals Assignment).
 * @version 1.0
 * @date 2021-02-20
 *
 * @copyright Copyright (c) 2021
 *
 */

#include "includes/details.h"
#include "includes/fort.hpp"
#include "includes/menus.h"
#include "includes/utils.h"
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdio.h>
#include <string>
#include <vector>

/**
 * @brief Get current date and time information.
 *
 * @return std::string
 */
std::string currentDateTime()
{
      // current date/time based on current system
      time_t now = time(0);

      // convert now to string form
      char *dateTime = ctime(&now);

      return dateTime;
}

/**
 * @brief Display the invoice of the purchase details for the user.
 *
 * @param details
 */
void generateInvoice(std::map<std::string, float> &details)
{
      std::string name = askName();

      std::string invoiceTitle = "Invoice No ";
      std::string invoiceNumber = std::to_string(rand());

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
            << name << fort::endr;

      // Row 3
      table << "Date"
            << currentDateTime() << fort::endr;

      // Row 4
      table << "Service Name"
            << "Price (Rs)"
            << "Units"
            << "Value" << fort::endr;

      for (auto const &[id, units] : details)
      {
            if (isPackage(id))
            {
                  float price = packagePriceLookup(id);

                  table << packageNameLookup(id)
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

      table[0][0].set_cell_span(2);
      table[0][1].set_cell_span(3);

      table[1][0].set_cell_span(2);
      table[1][1].set_cell_span(3);

      table[2][0].set_cell_span(2);
      table[2][1].set_cell_span(3);

      table[6][0].set_cell_span(3);
      table[7][0].set_cell_span(3);
      table[8][0].set_cell_span(3);

      printf("\n");
      printf("Thank you for your purchase!\n");
      printf("This is your receipt\n");
      printf("%s\n", table.to_string().c_str());
      printf("\n");
}
