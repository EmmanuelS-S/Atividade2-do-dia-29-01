
**Configuração do hardware**

-Os LEDs estão conectados aos pinos GPIO 11 (azul), 12 (vermelho) e 13 (verde), e o botão está conectado ao pino GPIO 5.
-Os pinos dos LEDs são configurados como saída (GPIO_OUT) e o pino do botão como entrada (GPIO_IN) com pull-up interno ativado.

--

**Comportamento do botão**

-Quando o botão é pressionado, a função de interrupção button_callback é acionada. Ela verifica se o botão foi pressionado com debounce (evitando múltiplas leituras rápidas).
-Se o botão for pressionado e nenhuma sequência estiver em andamento, a variável button_pressed é configurada como verdadeira.


--

**Sequência de LEDs**

-Quando o botão é pressionado, todos os LEDs acendem ao mesmo tempo.
 O temporizador é configurado para desligar os LEDs em sequência:
  Após 3 segundos, o LED verde é desligado.
  Após 6 segundos, o LED vermelho é desligado.
  Após 9 segundos, o LED azul é desligado.
-A variável sequence_running é usada para garantir que a sequência de desligamento dos LEDs não seja iniciada enquanto já estiver em andamento.

--

Temporizadores e Callbacks:

A função turn_off_callback é chamada com 3 alarmes temporizados, e desliga os LEDs em ordem (verde → vermelho → azul).
Cada alarme é configurado para ser executado com 3 segundos de intervalo, para desligar um LED de cada vez.
