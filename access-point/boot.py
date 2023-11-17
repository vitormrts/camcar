try:
    import usocket as socket
except:
    import socket
import network
import esp
esp.osdebug(None)
import gc
gc.collect()
ssid = 'ESP32'
password = '123456789'
ap = network.WLAN(network.AP_IF)
ap.active(True)
ap.config(essid=ssid, password=password)
while ap.active() == False:
    pass
print('Connection successful')
print(ap.ifconfig())
def web_page():
    html = """<html><head><meta name="viewport" content="width=device-width, initial-scale=1"></head>
<body style="background-color:powderblue;"><h1 style="text-align:center; font-size:70px;color:#ed0e42;">Linuxhint.com</h1><h2 style="text-align:center; font-size:30px;">ESP32 Access Point </h2></body></html>"""
    return html
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)
while True:
    conn, addr = s.accept()
    print('Connection established from %s' % str(addr))
    request = conn.recv(1024)
    print('Content = %s' % str(request))
    response = web_page()
    conn.send(response)
    conn.close()


