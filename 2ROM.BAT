rom_ext -KLRE
file_cv rom.bin rom_app.bin -C1 FF 12
rom_crc MODE_13 rom_app.bin
bin_hex /MI rom_tmp.hex rom_app.bin /CFF
copy /b pass_head.hex+rom_tmp.hex rom_app.hex