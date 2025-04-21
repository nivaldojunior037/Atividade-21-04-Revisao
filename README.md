# Atividade 21-04

Este é o repositório que armazena a tarefa solicitada no dia 15/04. Todos os arquivos necessários à execução já foram criados, de modo que basta seguir as instruções abaixo para executá-lo em seu dispositivo.

## Como Usar

1. Para acessar a atividade aramzenada, clone este repositório para seu dispositivo executando o seguinte comando no terminal de um ambiente adequado, como o VS Code, após criar um repositório local: 
git clone https://github.com/nivaldojunior037/Atividade-04-02-EmbarcaTech

2. Após isso, importe como um projeto a pasta que contém os arquivos clonados em um ambiente como o VS Code e compile o código existente para que os demais arquivos necessários sejam criados em seu dispositivo

3. Ao fim da compilação, será gerado um arquivo .uf2 na pasta build do seu repositório. Esse arquivo deve ser copiado para a BitDogLab em modo BOOTSEL para que ele seja corretamente executado. 

4. Abra o monitor serial do VS Code ou outro que você possua e selecione a entrada serial à qual sua placa foi conectada (caso não saiba qual é, consulte os dispositivos conectados nas configurações do seu computador). Selecione a taxa de 115200 e inicie o monitoramento.

### Como Executar o Código

1. Esse código foi projetado para monitorar o movimento do potenciômetro da placa BitDogLab e realizar uma contagem regressiva na matriz de LEDs quando o botão A é acionado. 

2. O quadrado no display é movimentado de acordo com a movimentação do potenciômetro. O único momento em que ele não está ativo é durante a emissão do sinal sonoro do buzzer.

3. O botão A aciona a contagem regressiva de 9 a 0 na matriz de LEDs 5x5. Essa contagem pode ser interrompida a qualquer momento por meio do acionamento do botão B. Quando a contagem se inicia, o LED RGB verde é acionado e o vermelho é desativado.

4. Quando a contagem se encerra, um sinal sonoro é emitido pelo buzzer e os desenhos da matriz são desligados. Além disso, o LED verde é desativado e o vermelho é ligado novamente até um novo acionamento do botão A. 

5. A contagem, bem como seu início, fim e interrupção, são todos mostrados pelo monitoramento serial realizado pelo VS Code ou qualquer outro método de monitoramento. A contagem pode ser reiniciada a qualquer instante por meio do botão A. 

#### Link do vídeo

Segue o link do Drive com o vídeo onde é demonstrada a utilização do código: https://drive.google.com/drive/folders/1rVEQVlkC1bqku78kuTpJ1QkfQ0nNSAfB?usp=sharing