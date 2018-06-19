import numpy as np
import tkinter as tk


def main():
      
  sudoku = [[s for s in input().split(' ')[:9]] for _ in range (9)]
  sudoku = np.asarray(sudoku)

  frame = tk.Tk()
  for row in range (9):
    for col in range (9):
      if (row in (0,1,2,6,7,8) and col in (3,4,5)) or (row in (3,4,5) and col in (0,1,2,6,7,8)):
        colour='lightgrey'
      else:
        colour='white'

      x=sudoku[row][col]
      if x == '0':
        x = ''

      btn=tk.Button(frame, width=4, height=3, bg=colour, text=x, font=30, fg='black')
      btn.grid(row=row, column=col, sticky=tk.N+tk.S+tk.E+tk.W)

  frame.mainloop()

main()
