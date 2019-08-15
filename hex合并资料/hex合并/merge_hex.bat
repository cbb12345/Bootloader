@echo off

merge\mergehex.exe --merge hex\app.hex                               hex\sn.hex         --output firmware.hex 

del hex\withoutsn.hex/s 


#pause
