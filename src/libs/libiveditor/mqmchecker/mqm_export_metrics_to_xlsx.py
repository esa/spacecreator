# This Python file uses the following encoding: utf-8

   #Copyright (C): GMV Aerospace and Defence, S.A.U.
   #Author: GMV Aerospace and Defence, S.A.U.

   #This library is free software; you can redistribute it and/or
   #modify it under the terms of the GNU Library General Public
   #License as published by the Free Software Foundation; either
   #version 2 of the License, or (at your option) any later version.

   #This library is distributed in the hope that it will be useful,
   #but WITHOUT ANY WARRANTY; without even the implied warranty of
   #MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   #Library General Public License for more details.

   #You should have received a copy of the GNU Library General Public License
   #along with this program. If not, see <https://www.gnu.org/licenses/lgpl-2.1.html>.


import sys

from xlsxwriter.workbook import Workbook



current_time=str(sys.argv[3])+" "+str(sys.argv[2])

workbookFile = Workbook(sys.argv[1]+ "/MQM_Dashboard_" + str(sys.argv[3]) + ".xlsx")
worksheet    = workbookFile.add_worksheet()

#Definition of Bold format
boldFormat = workbookFile.add_format({'bold': True})
boldFormat.set_center_across()
boldFormat.set_bg_color('#D9D9D9')
boldFormat.set_border(style=1)

#Definition of Cell format
cellFormat = workbookFile.add_format()
cellFormat.set_center_across()
cellFormat.set_border(style=1)

serialItem    = 6
numberRows    = int(sys.argv[4])
numberColumns = int(sys.argv[5])

#Write table got from TASTE MQM Model Checker
for row in range(numberRows):
    aux = 1
    for column in range(numberColumns):
        aux += 1
        if row == 0:
            worksheet.write(row, column, sys.argv[serialItem], boldFormat)
            if aux == numberColumns + 1:
              worksheet.write(row, column+1, "Metric Computation Time (UTC)", boldFormat )
        else:
            worksheet.write(row, column, sys.argv[serialItem], cellFormat)
            if aux == numberColumns + 1:
              worksheet.write(row, column+1, current_time,cellFormat)

        serialItem += 1

print(sys.argv[1]+ "/MQM_Dashboard_" + str(sys.argv[3]) + ".xlsx")

worksheet.autofit()
workbookFile.close()
