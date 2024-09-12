del new.hex
del rom.bin
copy .\New\debug\bin\New.hex new.hex
rom_ext -KLRE
pause
call file_cv rom.bin rom_app.bin -C1 FF 12
rom_crc MODE_13 rom_app.bin
bin_hex /MI rom_tmp.hex rom_app.bin /CFF
rem bin_hex /MI rom_tmp.hex rom_app.bin /S32 /CFF
copy head\pass_head.hex
copy /b pass_head.hex + rom_tmp.hex rom_app.hex
