import serial 
import serial.tools.list_ports
import tkinter as tk
import threading

class PortsWindow():
	def __init__(self):
	# left frame config
		self.root = tk.Tk()
		self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
		self.leftframe = tk.Frame(self.root, width = 250, height=500)
		self.leftframe.pack(side=tk.LEFT)
		separator = tk.Frame(height=2, bd=1, relief=tk.SUNKEN)
		separator.pack(fill=tk.Y, padx=5, pady=5)
		rightframe = tk.Frame(self.root, width=500, height=500)
		rightframe.pack(side=tk.LEFT)
		self.refresh = tk.Button(self.leftframe, 
								 text="Refresh",
								 command=self.get_ports)
		self.refresh.pack()
		self.menu = None
		self.var = tk.StringVar(self.leftframe)
		self.get_ports()
		self.menu.pack()
		
		baud_list = [300, 110, 150, 300, 1200, 2400, 4800, 9600, 19200, 38400, 
					 57600, 115200, 230400, 460800, 921600]
		self.baud_var = tk.StringVar(self.leftframe)
		self.baud_var.set(baud_list[0])
		self.baud_menu = tk.OptionMenu(self.leftframe, self.baud_var, *baud_list)
		self.baud_menu.pack()
		
		self.open = tk.Button(self.leftframe,
							  text="Open",
							  command=self.open_port)
		self.open.pack()
	# right frame config
		
	def get_ports(self):
		self.ports = [port.device for port in list(serial.tools.list_ports.comports())]
		if(len(self.ports) == 0):
			return
		self.var.set(self.ports[0])
		if(self.menu == None):
			self.menu = tk.OptionMenu(self.leftframe, self.var, *(self.ports))

		self.update_option_menu()
	
	def update_option_menu(self):
		menu = self.menu["menu"]
		menu.delete(0, "end")
		for string in self.ports:
			menu.add_command(label=string, 
					command=lambda value=string: self.var.set(value))
	
	def open_port(self):
		selected_port = self.var.get()
		baud = self.baud_var.get()
		serial_window = SerialWindow(selected_port, baud)
		serial_window.root.mainloop()
		
	def on_closing(self):
		self.root.destroy()
		
class SerialWindow():
	def __init__(self, port, baud):
		self.root = tk.Tk()
		self.root.protocol("WM_DELETE_WINDOW", self.on_closing)
		print(port)
		print(baud)
		self.cadence_text = tk.StringVar()
		self.strain_text = tk.StringVar()
		self.cadence = tk.Label(self.root, textvariable=self.cadence_text)
		self.strain = tk.Label(self.root, textvariable=self.strain_text)
		self.strain.pack()
		self.cadence.pack()
		
		self.close = False
		
		t = threading.Thread(target=self.read_serial, args=(port,baud,))
		t.start()
		
	def read_serial(self, port, baud):
		try:
			ser = serial.Serial(port=port, baudrate=baud, timeout=None)		
		except serial.serialutil.SerialException as e:
			print("Error: cannot open port ", port)
			return
		while not self.close:
			if(ser.inWaiting() > 0):
				line = ser.readline()
				print(line)
				if(line[0] == ord('c')):
					self.update_cadence(line[1:])
				elif(line[0] == ord('s')):
					self.update_strain(line[1:])
				else:
					print(line[0])
				self.root.update()
		ser.close()
		return
		
	def update_cadence(self, text):
		print(text)
		self.cadence_text.set("hello")
		
	def update_strain(self, text):
		self.strain_text.set("Hello")
		
	def on_closing(self):
		self.close = True
		self.root.destroy()

def main():
	start = PortsWindow()
	start.root.mainloop()

if __name__=="__main__":
	main()