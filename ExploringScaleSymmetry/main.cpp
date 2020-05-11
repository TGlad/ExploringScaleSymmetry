#include "stdafx.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
  vector<int (*)()> chapters[10];
  for (int i = 0; i < 10; i++)
    chapters[i].resize(26);
  // link up the chapter functions
  
  chapters[2][3] = &chapter2Figure3; chapters[2][4] = &chapter2Figure4; chapters[2][6] = &chapter2Figure6; chapters[2][10] = &chapter2Figure10; chapters[2][15] = &chapter2Figure15; chapters[2][16] = &chapter2Figure16; chapters[2][19] = &chapter2Figure19;
  chapters[3][1] = &chapter3Figure1; chapters[3][5] = &chapter3Figure5; chapters[3][9] = &chapter3Figure9; chapters[3][10] = &chapter3Figure10; chapters[3][12] = &chapter3Figure12; chapters[3][17] = &chapter3Figure17;
  chapters[4][9] = &chapter4Figure9; chapters[4][11] = &chapter4Figure11; chapters[4][20] = &chapter4Figure20;
  chapters[5][22] = &chapter5Figure22; chapters[5][25] = &chapter5Figure25;
  chapters[6][12] = &chapter6Figure12; chapters[6][17] = &chapter6Figure17;
  chapters[7][1] = &chapter7Figure1; chapters[7][3] = &chapter7Figure3; chapters[7][4] = &chapter7Figure4; chapters[7][6] = &chapter7Figure6; chapters[7][7] = &chapter7Figure7; chapters[7][12] = &chapter7Figure12; chapters[7][13] = &chapter7Figure13;
  chapters[8][2] = &chapter8Figure2;
  chapters[9][2] = &chapter9Figure2; chapters[9][4] = &chapter9Figure4; chapters[9][5] = &chapter9Figure5; chapters[9][12] = &chapter9Figure12;
  
  if (argc == 3)
  {
    int chapter = stoi(argv[1]);
    int figure = stoi(argv[2]);
    if (chapter >= 0 && chapter < 10 && figure >= 0 && figure < (int)chapters[chapter].size())
    {
      int returnVal = chapters[chapter][figure]();
      if (returnVal != 0)
        cout << "error processing chapter " << chapter << " figure " << figure << endl;
      return returnVal;
    }
  }
  cout << "Exploring Scale Symmetry: Figure Generation. Example usage:" << endl;
  cout << " generateFigure 2 3  -- generates Chapter 2, Figure 3." << endl;
  cout << "Figure options and parameters are at the top of each figure's .cpp file."
  cout << "The available figures to generate are:" << endl;
  for (int c = 0; c < 10; c++)
  {
    cout << "Chapter ";
    for (int f = 0; f < (int)chapters[c].size()-1; f++)
      cout << chapters[c][f] << ", ";
    if (chapters[c].size() > 0)
      cout << chapters[c].back() << endl;
  }
  return 1;
}
