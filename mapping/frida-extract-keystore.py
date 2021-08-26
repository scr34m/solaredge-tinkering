#!/usr/bin/python3

import frida, sys, time

app_name = 'com.solaredge.apps.activator'

def on_message(message, data):
    print(message)

jscode = """
setTimeout(function() {
    Java.perform(function () {

      var X = Java.use("h.a.a.a.d");
      X.b.overload("android.content.Context","java.lang.String","java.lang.String").implementation = function(var_0,var_1,var_2) {
        send("Var1 : " + var_1);
        send("Var2 : " + var_2);
        var ret = this.b.overload("android.content.Context","java.lang.String","java.lang.String").call(this,var_0,var_1,var_2);
        send("Ret : " + ret);
        return ret
      };

    });
},0);

"""

print("[.] Attaching to device...")
try:
    device = frida.get_usb_device()
except:
    print("[-] Can't attach. Is the device connected?")
    sys.exit()

print("[.] Spawning the app...")
try:
    pid = device.spawn(app_name)
    device.resume(pid)
    time.sleep(1)
except:
    print("[-] Can't spawn the App. Is filename correct?")
    sys.exit()

print("[.] Attaching to process...")
try:
    process = device.attach(pid)
except:
    print("[-] Can't connect to App.")
    sys.exit()

print("[.] Launching js code...")
print("  (run the app until needed, close it and then kill this script)")
script = process.create_script(jscode)
script.on('message', on_message)
script.load()
try:
	sys.stdin.read()
except KeyboardInterrupt:
    print ("\nExiting now")
    exit(0)
