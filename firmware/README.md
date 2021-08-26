Stripp off (proto buf data?) call `dd if=Version_4_13_40_173965.spff of=firmware0 skip=1 bs=64` and after make a symlink to `/tmp/files/firmware0`.

Calling `verde_re` will decode the XOR-ed files in the firmware0 file itself.

Not encoded files are, every thing else will be decoded:
 - sw-description
 - setup.sh
 - verde
 - se_add.pol
 - verde-run.sh

After `binwalk` and `ubireader_extract_files -o rootfs rootfs.ubifs` will do the rest.
