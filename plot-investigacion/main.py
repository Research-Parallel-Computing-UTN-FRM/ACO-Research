#! /usr/bin/python3
import os
import sys

import demjson
import matplotlib.pyplot as plt

PLOT_OUT = "plots"

values = None
# values {
#   sec_items int[] //miliseconds
#   par_items int[] //miliseconds
#   n_cities int[]
# }

for line in sys.stdin.readlines():
    values = demjson.decode(line)


print(values["sec_items"])

fig, ax = plt.subplots()
ax.set_title("Tiempso")
ax.plot(values["n_cities"], values["sec_items"])
# Add new line inside plot
# ax.plot(data["x"], data["coto"])
ax.set_xlabel("ciudades")
ax.set_ylabel("Tiempo [Miliseconds]")
ax.legend(["Secuencial"])

fig.tight_layout()

if not os.path.exists(os.path.join(os.getcwd(), PLOT_OUT)):
    os.mkdir(os.path.join(os.getcwd(), PLOT_OUT))

save_path = os.path.join(os.getcwd(), PLOT_OUT, "result" + ".png")
plt.savefig(save_path)
plt.close()
