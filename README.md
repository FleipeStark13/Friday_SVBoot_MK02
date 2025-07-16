# Friday Server – Controle do Servidor via Telegram

[![Instagram](https://img.shields.io/badge/Instagram-%23E4405F?style=for-the-badge&logo=instagram&logoColor=white)](https://instagram.com/fleipekkj.md)  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-%230A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/felipe-de-jesus-alves-liandro-415890222/)  
[![GitHub](https://img.shields.io/badge/GitHub-%23121011?style=for-the-badge&logo=github&logoColor=white)](https://github.com/FleipeStark13)  
[![Website](https://img.shields.io/badge/Website-00AEEF?style=for-the-badge&logo=web&logoColor=white)](https://felipeliandro.com.br)

---
<img width="1600" height="1060" alt="image" src="https://github.com/user-attachments/assets/d634d560-588c-4388-96d3-6f591751d52e" />
<img width="1200" height="1600" alt="image" src="https://github.com/user-attachments/assets/e14461ac-c94d-4382-8062-4e3f90d9fa12" />
<img width="2560" height="1981" alt="image" src="https://github.com/user-attachments/assets/16d306f5-9ba6-4c06-8ec9-d1ba18474ddd" />

---

## ⚙️ Sobre o Projeto

**Friday Server MK02** é um sistema de automação pessoal para gerenciamento de um servidor local, totalmente controlado via comandos no Telegram.

Com um ESP32, um bot personalizado e periféricos conectados, é possível ligar, desligar e reiniciar seu servidor remotamente, com feedback visual direto no display OLED e LEDs.

---

## 🧠 Funcionalidades e Fluxo

**Comandos disponíveis no Telegram:**
- `/pwp` — Liga o computador (Power Up)
- `/std` — Desliga o computador (Shut Down)
- `/reboot` — Reinicia o computador (via dois pulsos no relé)

**Fluxo inteligente com validação:**
- Os comandos só são aceitos se fizerem sentido: por exemplo, `/pwp` só funciona se o computador estiver desligado.
- Se o comando falhar, o **LED vermelho** pisca e o display mostra uma mensagem de **erro**.
- Se o comando for executado com sucesso, o **LED verde** pisca 4 vezes e o display exibe **mensagem de sucesso**.

---

## 🛠 Componentes Utilizados

- ESP32 DevKit V1
- Módulo Relé 5V com 2 canais
- 1 LED vermelho (erro)
- 1 LED verde (sucesso)
- Display OLED I2C 0.96" – 128x64px
- Bot Telegram integrado com ESP via WiFi

---

## 🧭 Lógica de Funcionamento

1. O ESP32 inicia e verifica status do computador.
2. Exibe no display se está **ligado ou desligado**.
3. Comandos via Telegram são validados:
   - Se válidos, o relé é acionado (pulso) e leds/displays reagem.
   - Se inválidos, feedback visual + resposta de erro no chat.
4. O usuário recebe mensagem de sucesso ou falha direto do bot.

---

## ✅ Projeto idealizado e construído por Felipe Liandro.

### Entre em contato:

[![Instagram](https://img.shields.io/badge/Instagram-%23E4405F?style=for-the-badge&logo=instagram&logoColor=white)](https://instagram.com/fleipekkj.md)  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-%230A66C2?style=for-the-badge&logo=linkedin&logoColor=white)](https://www.linkedin.com/in/felipe-de-jesus-alves-liandro-415890222/)  
[![GitHub](https://img.shields.io/badge/GitHub-%23121011?style=for-the-badge&logo=github&logoColor=white)](https://github.com/FleipeStark13)  
[![Website](https://img.shields.io/badge/Website-00AEEF?style=for-the-badge&logo=web&logoColor=white)](https://felipeliandro.com.br)
