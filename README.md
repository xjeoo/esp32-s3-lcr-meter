# ESP32-S3 LCR Meter

Un instrument de masura pentru Inductanta (L), Capacitate (C) si Rezistenta (R) bazat pe microcontrolerul ESP32-S3 si framework-ul Arduino (PlatformIO).

## 🚧 Stadiul Proiectului: Work In Progress
Momentan, proiectul este in faza de dezvoltare. 

### Ce functioneaza acum:
- [x] Configurarea mediului de dezvoltare PlatformIO.
- [x] Generarea semnalului de test prin PWM/Timere (frecventa fixa).

### Ce urmeaza:
- [ ] Implementarea citirii ADC pentru esantionarea semnalului.
- [ ] Calculul defazajului si al impedantei.
- [ ] Interfata grafica pe Serial/OLED.
- [ ] Schema hardware finala.

## 🛠️ Cum se ruleaza
1. Instaleaza VS Code + extensia PlatformIO.
2. Cloneaza acest repo.
3. Conecteaza ESP32-S3.
4. Apasa pe **Upload** in PlatformIO.