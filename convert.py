from re import A
import tkinter
import tkinter.ttk
import tkinter.messagebox
import sys


def bin2dec(value):
	return int(value, 2)


def bin2hex(value):
	return hex(int(value, 2))


def dec2bin(value):
	return bin(int(value))


def dec2hex(value):
	return hex(int(value))


def hex2bin(value):
	return bin(int(value, 16))


def hex2dec(value):
	return int(value, 16)


class MainWindow():

	C_FONT = ("Consolas", 14)
	C_TXT_MAXLEN = 32

	def __init__(self):
		self._window = tkinter.Tk()
		self._window.title("Convert Program Python")
		self._window.geometry("480x240")
		self._window.resizable(False, False)

		self.OPTIONS = ["Hexadecimal", "Decimal", "Binary"]

		label = tkinter.Label(self._window, text="From:", font=MainWindow.C_FONT)
		label.grid(row=0, column=0,  padx=10, pady=5)

		self.source_val = tkinter.StringVar()
		self.source_val.set(self.OPTIONS[0])

		option_1 = tkinter.OptionMenu(self._window, self.source_val, *self.OPTIONS)
		option_1.grid(row=0, column=1, padx=10, pady=5)

		label = tkinter.Label(self._window, text="To:", font=MainWindow.C_FONT)
		label.grid(row=1, column=0,  padx=10, pady=5)

		self.destination_val = tkinter.StringVar()
		self.destination_val.set(self.OPTIONS[1])

		option_2 = tkinter.OptionMenu(
			self._window, self.destination_val, *self.OPTIONS)
		option_2.grid(row=1, column=1, padx=10, pady=5)

		label = tkinter.Label(self._window, text="Number: ", font=MainWindow.C_FONT)
		label.grid(row=2, column=0, padx=10, pady=5)

		self._txt_input = tkinter.Entry(
			self._window, width=MainWindow.C_TXT_MAXLEN, font=MainWindow.C_FONT)
		self._txt_input.grid(row=2, column=1, pady=5)
		self._txt_input.focus()

		self._bt_bin = tkinter.Button(
			self._window, text="Convert", font=MainWindow.C_FONT, command=self.convert)
		self._bt_bin.grid(row=3, column=1, padx=10, pady=5)

		separator = tkinter.ttk.Separator(self._window, orient=tkinter.HORIZONTAL)
		separator.grid(row=3, column=1, pady=10)

		label = tkinter.Label(self._window, text="Result:", font=MainWindow.C_FONT)
		label.grid(row=4, column=0,  padx=10, pady=5)

		self._stringvar_result = tkinter.StringVar()
		txt_output = tkinter.Entry(self._window, textvariable=self._stringvar_result,
		                           width=MainWindow.C_TXT_MAXLEN, state="readonly", font=MainWindow.C_FONT)
		txt_output.grid(row=4, column=1, pady=5)

	def convert(self):
		try:
			if(self.source_val.get() == self.OPTIONS[0]):
				hex_value = self._txt_input.get().strip().replace(" ", "")
				if(self.destination_val.get() == self.OPTIONS[1]):
					value_result = hex2dec(hex_value)
					self._set_values(value_result)
				elif(self.destination_val.get() == self.OPTIONS[2]):
					value_result = hex2bin(hex_value)
					self._set_values(value_result)
				else:
					self._set_values(hex_value)

			elif(self.source_val.get() == self.OPTIONS[1]):
				dec_value = self._txt_input.get().strip().replace(" ", "")
				if(self.destination_val.get() == self.OPTIONS[0]):
					value_result = dec2hex(dec_value)
					self._set_values(value_result)
				elif(self.destination_val.get() == self.OPTIONS[2]):
					value_result = dec2bin(dec_value)
					self._set_values(value_result)
				else:
					self._set_values(dec_value)

			else:
				bin_value = self._txt_input.get().strip().replace(" ", "")
				if(self.destination_val.get() == self.OPTIONS[1]):
					value_result = bin2dec(bin_value)
					self._set_values(value_result)
				elif(self.destination_val.get() == self.OPTIONS[0]):
					value_result = bin2hex(bin_value)
					self._set_values(value_result)
				else:
					self._set_values(bin_value)
		except Exception as ex:
			tkinter.messagebox.showerror("Error", "Invalid conversion")
			print(ex, file=sys.stderr)

	def _set_values(self, value):
		self._stringvar_result.set(value)

	def mainloop(self):
		self._window.mainloop()


if __name__ == "__main__":
	win = MainWindow()
	win.mainloop()
