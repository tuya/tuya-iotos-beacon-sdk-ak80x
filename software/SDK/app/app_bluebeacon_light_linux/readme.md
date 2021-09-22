
### 1.build

```
cd _build
bash run.sh build config.json
```

</br>

### 2.run

it needs to install bluez (>5.0) for running this application.

how to install: [Download_install_and_use_the_BlueZ_and_hcitool_on_PI_3B+.md](./doc/Download_install_and_use_the_BlueZ_and_hcitool_on_PI_3B+.md)

check whether BlueZ is successfully installed with the following command:

```
sudo hciconfig hci0 up
hcitool scan
```

</br>


```
cd _build
sudo ./app_main
```
