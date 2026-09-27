# Radar de semânticas de interação musical

**Status:** pesquisa de design, não API nem roadmap

**Última revisão:** 2026-09-25

## Propósito

Este radar guarda comportamentos encontrados em instrumentos e softwares
musicais para ajudar a nomear futuras fronteiras do ecossistema. Ele evita tanto
reinventar vocabulário estabelecido quanto transformar a primeira implementação
útil numa abstração compartilhada cedo demais.

As referências externas mostram possibilidades, não requisitos. A promoção de
um conceito deve ser guiada por experimentos nossos e por julgamento
arquitetural; um limite promissor pode ser implementado como módulo
experimental antes de haver dois consumidores ou um contrato final.

## Estados

| Estado | Significado |
| --- | --- |
| Referência | comportamento documentado em outro instrumento ou software |
| Observado | comportamento exercitado em um experimento nosso |
| Candidato | possibilidade relevante para um próximo experimento concreto |
| Adotado | semântica deliberadamente usada por uma composição local |
| Extraído | contrato promovido para pacote compartilhado |
| Adiado | possibilidade preservada sem prioridade atual |

Um conceito pode aparecer em mais de uma camada e ter estados diferentes. Por
exemplo, trigger já é uma prática observada no probe de bateria AMY, mas ainda
não foi extraído como contrato compartilhado.

## Radar atual

| Conceito | Camada provável | Referências externas | Evidência ou uso nosso | Estado atual |
| --- | --- | --- | --- | --- |
| Trigger / one-shot | evento e reprodução | Koala, Roland P-6, Pocket Operators, drum machines | probe do drum kit AMY; sequenciador Calculator dispara quatro tracks AMY | observado, candidato |
| Gate | evento e voz | MIDI, Koala, Roland P-6 | Note On/Off nos showcases BLE MIDI | observado |
| Hold / latch | interação e voz | Koala, Roland P-6 e Behringer Crave | nenhum experimento dedicado | referência |
| Loop / toggle | política de reprodução | Koala e Roland P-6 | nenhum experimento dedicado | referência |
| Choke | política entre vozes | samplers e hi-hats de drum machines | ainda não testado | referência |
| Monofonia / polifonia | alocação de vozes | MIDI, AMY e Roland P-6 | instrumento monofônico e slot AMY | adotado localmente |
| Mute / mute all | política de pista | sequenciadores e Roland P-6 | interação planejada para Calculator | candidato |
| Pattern | composição | sequenciadores e Roland P-6 | Calculator implementa quatro patterns voláteis de quatro tracks por dezesseis steps | adotado localmente |
| Sequence slot | armazenamento e seleção | 1010music Nanobox Razzmatazz | Calculator valida quatro slots de pattern selecionáveis | adotado localmente |
| Note / rest | conteúdo do step | Behringer Crave e sequenciadores melódicos | Calculator implementa rest ou trigger em três intensidades, sem pitch por step | adotado localmente para bateria |
| Velocity / accent | evento e step | MIDI, Koala, Roland P-6 e Behringer Crave | velocity nos contratos MIDI; Calculator valida weak/normal/strong por step e os converte em velocity no sink AMY | extraído para notas; adotado localmente para steps |
| Gate time | step e voz | Roland P-6 e Behringer Crave | ainda não necessário para bateria one-shot | adiado |
| Probability | step | Roland P-6 | ainda não testado | referência |
| Micro-timing | agendamento | Roland P-6 | ainda não testado | referência |
| Sub-steps / ratchet | expansão de step | Roland P-6 e Behringer Crave | ainda não testado | referência |
| Motion | automação de parâmetros | Roland P-6 | CC contínuo existe, mas não é sequenciado | referência |
| Parameter lock | automação por step | Pocket Operators | ainda não testado | referência |
| Punch-in effect | performance temporária | Pocket Operators | ainda não testado | referência |
| Accent | expressão do step | Pocket Operators e drum machines | Calculator edita três níveis por modificadores A/B e reproduz três intensidades | adotado localmente |
| Swing | clock e agendamento | Pocket Operators, Roland P-6 e Korg Volca | Calculator valida 50--75%, pares long/short, bypass em rates `T`; mecanismo físico alternado extraído em `musical-clock` | adotado localmente; mecanismo extraído |
| Step length / step count | geometria temporal | 1010music Nanobox Razzmatazz | Calculator fixa 16 steps e um rate global | referência; candidato |
| Quantized sequence change | arranjo e transporte | 1010music Nanobox Razzmatazz | Calculator troca patterns na fronteira de dezesseis steps | adotado localmente |
| Sequence copy / paste / double | edição e variação | 1010music Nanobox Razzmatazz | Calculator clona o pattern atual para outro slot; double continua futuro | adotado localmente para copy |
| Per-step velocity | expressão do evento | 1010music Nanobox Razzmatazz | Calculator armazena `StepLevel` weak/normal/strong e mapeia para velocity | adotado localmente |
| Step Loop / Scatter | performance sobre transporte | Roland P-6 | ainda não testado | referência |
| Active Step | geometria do pattern | Korg Volca | todos os 16 steps implementados permanecem ativos | referência |
| Step Jump | performance sobre transporte | Korg Volca | troca de pattern planejada é quantizada, não um jump | referência |
| Flux / gravação livre | captura temporal | Korg Volca Keys | ainda não testado | referência |
| Step Trigger | articulação da voz | Korg Volca Keys | ainda não testado | referência |
| Stutter | repetição performática | Korg Volca Beats | ainda não testado | referência |
| Pattern end / logical length | estrutura do pattern | Behringer Crave | Calculator implementa 16 steps fixos | adotado localmente |
| Page | navegação de edição | Behringer Crave | Calculator expõe os 16 steps simultaneamente | referência |
| Reset / hold playhead | performance sobre transporte | Behringer Crave | ainda não testado | referência |
| Arpeggiator traversal | geração algorítmica | Behringer Crave | ainda não testado | referência |
| Composition hierarchy | arranjo e armazenamento | Teenage Engineering EP-133 K.O. II | Calculator combina tracks diretamente em patterns | referência |
| Pattern bank / preset scope | ciclo de vida e armazenamento | 1010music Nanobox Razzmatazz | Calculator ainda inicia com um único pattern volátil | referência |
| Commit / variation | workflow de composição | Teenage Engineering EP-133 K.O. II | ainda não testado | referência |
| Scene duration policy | sincronização de patterns | Teenage Engineering EP-133 K.O. II | patterns atuais teriam duração uniforme | referência |
| Parameter scope | estado e automação | Teenage Engineering EP-133 K.O. II | ainda não formalizado | referência |
| Retrigger / legato continuation | articulação da voz | Teenage Engineering EP-133 K.O. II | synth BLE usa retrigger monofônico | referência |
| Selective quantization | edição temporal | Teenage Engineering EP-133 K.O. II | ainda não testado | referência |
| Undo | edição | Teenage Engineering EP-133 K.O. II e Pocket Operators | ainda não planejado | referência |
| Loop window | performance sobre timeline | Teenage Engineering EP-133 K.O. II | ainda não testado | referência |
| Quantized command | agendamento de interação | Teenage Engineering EP-133 K.O. II | troca de pattern planejada para a fronteira | candidato |
| Pressure-sensitive effect | expressão de performance | Teenage Engineering EP-133 K.O. II | hardware Calculator não mede pressão | referência |
| Tracker hierarchy | composição por referências | picoTracker e MicroDexed Touch | ainda não testado | referência |
| Command table | automação reutilizável | picoTracker | ainda não testado | referência |
| Overdub checkpoint | gravação incremental | Wavy Industries Monkey | ainda não testado | referência |
| Nondestructive modifier / mixdown | edição e renderização MIDI | RetroKits RK-008 | ainda não testado | referência |
| Track / MIDI-channel independence | roteamento | RetroKits RK-008 | canais selecionam patches AMY, mas não tracks | referência |
| Parallel sequencer models | composição | MicroDexed Touch | ainda não testado | referência |
| Conditional / generative step | decisão do sequenciador | Woovebox | ainda não testado | referência |
| Gesture mapping | expressão de performance | Donner MEDO | ainda não testado | referência |
| Phrase looper | captura de performance | Yamaha Reface CS/DX | ainda não testado | referência |
| Record-defined loop length | estrutura temporal | Yamaha Reface CS/DX e RK-008 | ainda não testado | referência |
| Volatile sketchpad | ciclo de vida da composição | Yamaha Reface CS/DX | ainda não testado | referência |
| Control-value lane | sequenciamento de modulação | Moog DFAM | ainda não testado | referência |
| Clock / value decoupling | agendamento | Moog Subharmonicon | ainda não testado | referência |
| Rhythm generator | geração de pulsos | Moog Subharmonicon | ainda não testado | referência |
| Remote projection / viewport | interface | Akai APC40 mkII | futura UI M5 ainda não possui host externo | referência |
| Bidirectional controller feedback | interface e estado | Akai APC40 mkII | ainda não testado | referência |
| Clip / scene launch | performance de arranjo | Ableton via Akai APC40 mkII | ainda não testado | referência |
| Transformation pipeline | processamento de eventos | Arturia KeyLab mk3 | ainda não testado | referência |
| Tie / legato transition | articulação do step | Arturia KeyStep e Moog Grandmother | ainda não testado | referência |
| Polyphonic / chord step | conteúdo do step | Moog Matriarch | ainda não testado | referência |
| Pattern chain | arranjo | Pocket Operators | Calculator valida doze posições visíveis com slots de pattern atribuídos | adotado localmente |
| Chop | material sonoro e mapeamento | Roland P-6 | ainda não testado | referência |
| Resampling | criação de material | Koala e Roland P-6 | ainda não testado | referência |
| Síntese granular | engine sonora | Roland P-6 | AMY é outra classe de engine | referência |
| Clock / transport | tempo e execução | MIDI e sequenciadores | metronome e Calculator validam deadline absoluto; `musical-clock` extrai clocks periódico e alternado, enquanto transport permanece nos apps | mecanismo de clock extraído; transport candidato |
| External clock priority | sincronização | 1010music Nanobox Razzmatazz | ainda não testado em nossos instrumentos | referência; candidato |
| Program / bank | seleção de sons | MIDI e instrumentos digitais | patches AMY selecionados por canal | referência; sem contrato próprio |

## Fronteiras sugeridas pela comparação

Os produtos estudados reforçam que o gesto, o evento e a reprodução não são a
mesma coisa:

```text
controle físico -> edição ou performance -> evento musical -> política da voz
                                               |
sequenciador -> decisão temporal ---------------+
```

- **Trigger** informa que algo deve começar agora. A duração pode ser autônoma,
  como num sample one-shot ou numa voz de bateria.
- **Gate** possui começo e fim significativos. O release pode encerrar a voz ou
  iniciar a fase de release do envelope.
- **Hold, latch e toggle** convertem um gesto momentâneo em estado persistente.
- **Loop, choke, envelope e polifonia** são políticas do instrumento ou da voz;
  não precisam inflar o evento de trigger.
- **Probability, micro-timing, sub-steps e motion** enriquecem decisões do
  sequenciador; não são propriedades obrigatórias de todo evento musical.

Por isso, um possível `TriggerEvent` deve permanecer diferente de Note On/Off.
O MIDI pode transportar bateria através de Note On, mas isso não obriga a
composição interna a fingir que todo disparo possui um release musicalmente
relevante.

## Consequência para a Calculator Face

O painel original da Calculator Face entrega eventos orientados ao release.
Essa limitação impede uma execução gate fiel pelas teclas da Face, mas combina
bem com edição de steps, disparos one-shot e comandos toggle. Os botões A, B e C
do Core Gray continuam disponíveis quando uma interação precisar distinguir
press e release.

O repositório
[`calculator-face-input`](https://github.com/embedded-music/calculator-face-input)
validou essa hipótese em hardware. Seu baseline possui quatro tracks, quatro
patterns de dezesseis steps, chain, playhead contínuo, triggers one-shot AMY,
três intensidades, BPM, rate, swing e volume ajustáveis, e seleção direta de
vinte sons por track. A grade mantém edição normal sem modificador; Core A abre
settings e Core B abre seleção de som.

O app agora possui quatro patterns voláteis, chain de doze posições, clone e
clear de pattern, três intensidades por step, rates retos/triplets e swing.
Mute, persistência, probability, sub-steps e micro-timing continuam no radar
sem inflar contratos compartilhados antes de existir evidência musical e de
interface.

## Pocket Operators como família

Os Pocket Operators são especialmente próximos da Calculator Face por obterem
vários papéis de uma grade de dezesseis teclas e poucos controles auxiliares.
Ainda assim, a família não define um único instrumento ou contrato. O núcleo
recorrente é um pattern de dezesseis steps, seleção de som, modo de escrita,
gravação ao vivo quantizada, swing, parameter locks, efeitos performáticos e
encadeamento de patterns. As especializações mudam a semântica dessa superfície:

- o **PO-32 tonic** trata a grade como sons, steps ou efeitos conforme o modo;
  oferece accent, step multiplier, mute/solo, parameter locks, efeitos punch-in
  e chains de até 64 patterns;
- o **PO-33 K.O!** separa slots melódicos, nos quais as teclas transpõem o sample
  inteiro, de slots de bateria, nos quais as teclas selecionam slices; start e
  duração podem ser editados por slice;
- o **PO-20 arcade** acrescenta seleção e chaining de acordes, além de pattern
  chaining e step multiplier, mostrando que uma segunda sequência pode dirigir
  contexto harmônico em vez de triggers de bateria.

Essa variedade reforça duas ideias para o nosso design. Primeiro, a grade física
é uma superfície modal: seu significado pertence ao estado da composição, não
ao driver do teclado. Segundo, pattern chain, parameter lock, multiplier e
punch-in effect são operações diferentes, embora compartilhem as mesmas teclas.
Não devemos reuni-las num contrato genérico apenas por terem interface física
parecida.

## Korg Volca como família

Os Volcas acrescentam principalmente operações performáticas sobre a geometria
e a posição temporal do pattern. Assim como nos Pocket Operators, recursos
variam entre modelos e não formam uma única API de sequenciador.

- **Active Step** desativa posições que passam a ser puladas durante playback e
  gravação. Isso é diferente de um rest: o rest ocupa tempo sem disparar, enquanto
  um step inativo deixa de participar do ciclo.
- **Step Jump** desloca imediatamente o playhead para uma posição pressionada.
  Isso é diferente da troca de pattern quantizada planejada para a Calculator.
- **Flux**, no Volca Keys, grava e reproduz continuamente sem alinhar a execução
  ao início de um step. Com Flux desligado, a gravação volta a ser quantizada.
- **Step Trigger**, também no Volca Keys, força o envelope a reiniciar no começo
  do step. A articulação pode, portanto, ser controlada separadamente da nota.
- **Stutter**, no Volca Beats, repete o trigger de uma parte com intervalo e
  decaimento ajustáveis. Ele ocupa uma fronteira diferente do step multiplier:
  é uma transformação performática, embora ambos possam produzir repetições.
- **Choke configurável**, no Volca Drum, determina quais partes interrompem umas
  às outras e reforça choke como política entre vozes.
- **Sync por pulso** permite que um pulso externo avance um step, exemplificando
  um clock mínimo que não precisa carregar notas nem o protocolo MIDI completo.

A distinção introduzida por Active Step pode ser representada conceitualmente
por três estados, sem sugerir que o primeiro sequenciador deva implementá-los:

```text
inactive -> posição removida do ciclo
rest     -> posição consome tempo sem disparar
trigger  -> posição consome tempo e dispara
```

Flux revela outra fronteira potencial: captura quantizada produz naturalmente
um pattern de steps, enquanto captura livre exige uma timeline ou timestamps.
Essa diferença deve permanecer no radar até algum experimento nosso precisar
gravar performance, pois o sequenciador Calculator edita steps e atribuições de
som, mas não grava uma execução ao vivo.

## 1010music Nanobox Razzmatazz

O Razzmatazz é uma referência particularmente próxima do próximo passo do
sequenciador Calculator: uma caixa de bateria compacta que combina pads,
sequenciamento, síntese e samples numa mesma composição. A documentação oficial
descreve oito pads, dezesseis slots de sequence por preset e uma tela de edição
que mostra todos os pads enquanto mantém um pad em foco.

As decisões mais úteis para o nosso radar são:

- **Step length e step count são separados.** Cada sequence pode ter uma
  divisão própria (de 1/64 a vários compassos) e de 1 a 64 steps. A duração
  musical é `step length × step count`, em vez de ser implicitamente sempre
  dezesseis posições.
- **Troca de sequence é quantizada.** Ao selecionar outra sequence durante a
  reprodução, ela começa quando a atual termina. Isso dá uma semântica clara
  para `queue next pattern`, diferente de um jump imediato do playhead.
- **Copy, paste, clear e double são operações de sequence.** A cópia inclui
  triggers, configuração temporal e velocities; `Double` duplica o material no
  fim. Isso separa variação estrutural de editar cada step individualmente.
- **Gravação e edição são complementares.** Finger drumming e MIDI podem gravar
  eventos em tempo real, quantizados ao step mais próximo, preservando velocity;
  o Super Stepper permite corrigir triggers e velocity por pad e step depois.
- **Swing é uma transformação do agendamento.** Ele desloca os steps pares de
  1/16 em torno da posição reta e pode ser ouvido em tempo real, sem alterar o
  conteúdo do pattern.
- **O clock tem prioridade de fontes.** A ordem documentada é Clock In
  analógico, depois MIDI Clock, depois BPM interno. Isto sugere que uma futura
  camada de clock deve separar fonte, fallback e autoridade de tempo, em vez de
  tratar `BPM` como a única origem possível.
- **Preset é um escopo de composição.** Pad settings, mixer, efeitos, sequences,
  sequence default, MIDI mappings e modulação são salvos juntos, explicitamente,
  em cartão SD. Isso contrasta com o nosso pattern volátil e oferece uma
  referência para separar estado de edição, material armazenado e transporte.

O Razzmatazz não é uma especificação para copiar: sua tela touchscreen, seu
modelo de samples/FM e seu armazenamento em SD resolvem problemas maiores que os
da Calculator. A contribuição principal é semântica: `sequence` pode ser um
objeto armazenável e selecionável, `pattern end` pode ser independente da grade
visível, e uma troca futura pode ser solicitada agora para começar numa fronteira
musical. Essas ideias entram como candidatos locais para o próximo slice de
patterns, sem alterar ainda o contrato do clock compartilhado.

## Behringer Crave e o step melódico

O Crave complementa as drum machines ao mostrar o conteúdo necessário para
sequenciar uma voz melódica monofônica. Seu sequenciador armazena notas e rests
em patterns de até 32 steps, organizados visualmente em quatro páginas, e permite
ajustar gate length, accent, ratchet e o fim lógico do pattern.

Um step de bateria one-shot pode inicialmente responder apenas se deve disparar
uma pista. Um step melódico precisa separar mais dimensões:

```text
posição temporal
  -> note ou rest
  -> pitch, quando for note
  -> gate length
  -> accent
  -> ratchet
```

Isso não justifica uma estrutura `Step` universal. Pelo contrário, sugere que um
futuro pattern reutilizável precisaria ser parametrizado pelo tipo de conteúdo,
ou que `DrumStep` e `MonophonicNoteStep` deveriam permanecer modelos distintos.
A decisão pode ser explorada diretamente no próximo consumidor; uma fronteira
comum experimental pode ser criada e revisada conforme a implementação avance.

O Crave também ajuda a separar conceitos que uma interface compacta aproxima:

- **page** escolhe qual parte de um pattern longo está visível para edição;
- **pattern end** define seu comprimento musical lógico;
- **bank/pattern** seleciona material armazenado;
- **reset** volta imediatamente ao primeiro step;
- **hold step** mantém temporariamente a posição atual;
- **arpeggiator** gera uma ordem algorítmica a partir de notas mantidas, em vez
  de simplesmente reproduzir notas previamente gravadas no pattern.

As saídas físicas separadas de keyboard CV e gate reforçam ainda que pitch e
articulação são dimensões independentes. Um pitch pode permanecer estável
enquanto novos gates rearticulam a voz; um rest pode fechar o gate sem precisar
apagar o último pitch.

Glide/portamento existe no sintetizador, mas a documentação consultada não é
suficiente para afirmar `tie` ou slide programável por step. Esses comportamentos
não entram no radar como capacidades confirmadas do sequenciador Crave.

## Teenage Engineering EP-133 K.O. II

O K.O. II acrescenta uma hierarquia de composição mais explícita que os
instrumentos anteriores:

```text
project
  -> quatro groups com samples e patterns próprios
  -> scenes que combinam os patterns correntes
  -> song positions que referenciam scenes em ordem
```

Essa hierarquia distingue dados musicais de suas combinações. Uma scene pode
referenciar patterns de groups diferentes; uma song position referencia a scene
em vez de copiar novamente seu conteúdo. A duração da song position segue o
pattern mais longo da scene, tornando a política de duração parte explícita do
arranjo.

O comando **commit** preserva a combinação atual como scene e cria uma nova
variação baseada nela sem parar a música. Ele funciona como checkpoint criativo
e duplicate-and-continue, não apenas como persistência em armazenamento.

O K.O. II também torna explícito o escopo dos parâmetros. Volume de group no
project, level automatizável no pattern, configuração do sample, send de efeito
por group e processamento master possuem nomes parecidos, mas ciclos de vida e
capacidades de automação diferentes. Uma futura API não deveria oferecer um
`setVolume()` ambíguo sem identificar seu alvo e escopo.

Os modos de sample separam outras políticas de voz:

- **oneshot** é monofônico e toca o sample inteiro;
- **key** permite múltiplas instâncias polifônicas do mesmo sample;
- **legato** é monofônico e muda a nota preservando a posição de reprodução.

Legato demonstra que uma nova nota não precisa reiniciar cursor e articulação.
Retrigger ou continuação pertencem à política da voz, não ao Note On isolado.

Na edição temporal, o aparelho diferencia captura quantizada, free time,
correção seletiva de uma nota ou pad, deslocamento dentro ou fora da grade e
undo. Quantização aparece assim como transformação com escopo, e não apenas
configuração global do clock.

Automação de fader gravada num step é latching: o valor permanece até ser
substituído. Punch-in effects respondem à pressão e podem ser combinados. O modo
Loop possui posição e comprimento próprios e permite saída imediata ou na
próxima barra. Essa última escolha exemplifica um **quantized command**: o gesto
ocorre agora, mas a mudança de estado é aplicada numa fronteira musical.

Para a Calculator, `queue next pattern` continua candidato local dessa
semântica, ainda não implementado. Isso não exige adotar a hierarquia completa
do K.O. II: o primeiro pattern já contém diretamente quatro tracks de dezesseis
steps.

## Survey de tiny sequencers

O vídeo *Tiny Sequencers Compared*, de Floyd Steinberg, foi usado como índice de
descoberta para uma família mais ampla de instrumentos compactos. Afirmações
técnicas abaixo foram confirmadas, quando possível, nos manuais e páginas dos
respectivos projetos; o vídeo não é tratado como especificação.

### Wavy Industries Monkey: loop por overdubs

O Monkey favorece captura imediata em vez de edição de uma grade. Seu loop
recorder possui comprimento fixo de 16 beats e cria um checkpoint ao terminar
cada overdub. Undo remove o último checkpoint; uma ação mais ampla apaga um canal
MIDI inteiro. Mute por canal permite transformar as camadas gravadas num arranjo
performático simples.

Esse modelo separa `take/overdub` de `track`: o histórico de criação é composto
por passagens de gravação, enquanto organização e mute usam canais MIDI. Também
mostra uma política útil de deduplicação ao atravessar a fronteira do loop: notas
periódicas idênticas sobrepostas não são gravadas novamente.

### picoTracker: referências e comandos verticais

O picoTracker usa a hierarquia clássica de tracker: song channels referenciam
chains, chains referenciam phrases e phrases armazenam eventos em rows. Uma
chain pode reutilizar a mesma phrase com transposição diferente, reduzindo cópia
de dados musicais.

Tables formam pequenas sequências reutilizáveis de comandos, disparadas por uma
phrase ou instrumento. Elas podem executar automação, arpejos e efeitos, fazer
loop ou saltar entre trechos. Isso sugere uma fronteira entre conteúdo musical e
programas de modificação, mas não exige uma linguagem de comandos no nosso
sequenciador.

### RetroKits RK-008: performance MIDI transformável

O RK-008 grava performance MIDI em tempo real. Uma track pode conter eventos de
qualquer um dos 16 canais, e canal de saída e porta são decisões de roteamento
separadas. Track, canal MIDI e instrumento não são, portanto, sinônimos.

Volume, transpose, quantize e note length podem ser aplicados como modificadores
não destrutivos. `Mixdown` materializa essas transformações nos eventos, após o
que novas transformações podem ser empilhadas. `Merge` combina tracks ou parts;
`explode` divide conteúdo MIDI em várias tracks. O comprimento de uma part pode
ser inferido da performance gravada em vez de definido antes da captura.

Esse workflow distingue claramente:

```text
eventos gravados -> transformação não destrutiva -> render/mixdown
```

### MicroDexed Touch: dois sequenciadores sincronizados

O MicroDexed Touch mantém dois modelos que podem funcionar simultaneamente: um
sequenciador inspirado em LSDJ, estruturado em song, chains e patterns, e um
LiveSequencer voltado à gravação mais direta. A coexistência é evidência de que
tracker e gravador linear atendem atividades diferentes e não precisam ser
fundidos numa abstração única para compartilhar clock e engines sonoras.

Seu sequenciador por patterns também permite reutilizar dados com instrumentos
e transposições diferentes. O conteúdo musical pode ser uma referência
independente da voz que o executará.

### Woovebox: steps como pequenas decisões

O Woovebox amplia o step com conditionals, probability e mais de cem tipos de
modificadores, além de polirritmia, geração, randomização de patterns/chains e
transformações por fragmento no song mode. Ele representa o extremo oposto do
step booleano inicial da Calculator: cada posição pode funcionar como uma
pequena decisão programável.

Essa referência reforça a cautela do radar. Um modelo poderoso de `Step` não é
automaticamente um bom contrato comum; complexidade deve aparecer apenas quando
um experimento musical precisar dela.

### Donner MEDO: looper modal e gestos

O MEDO organiza performance em papéis como drum, bass, chord, lead e sample e
usa um looper com overdub, quantização e até 128 bars. Sensores permitem mapear
click, press, slide, slap, tilt, shake, wiggle e movimento para parâmetros.

Sua contribuição principal ao radar é a separação entre evento discreto e gesto
contínuo ou corporal. `Gesture mapping` pertence à borda de controle; o resultado
pode virar nota, modulação ou comando sem exigir que o instrumento conheça o
sensor físico.

### Itens que não entram como sequenciadores

O computador Elecrow com Raspberry Pi é uma plataforma geral capaz de hospedar
software musical, não uma semântica própria de sequenciamento. Geonkick, Surge
XT, ZynAddSubFX e TAL Noisemaker aparecem no setup como engines ou instrumentos
de software. Eles podem alimentar outra pesquisa sobre síntese e áudio, mas não
são usados como evidência deste survey de workflows.

## Survey de sequenciadores embarcados em synths

Uma segunda tier list em vídeo foi usada como índice para sequenciadores e
controles integrados a sintetizadores. Apenas os modelos classificados pelo
autor nos tiers A ou S foram examinados. O ranking é opinião editorial; as
semânticas abaixo vêm dos manuais oficiais dos fabricantes.

### Yamaha Reface CS/DX: phrase looper efêmero

O Phrase Looper inicia a primeira gravação com o primeiro key-on e toma a duração
executada como comprimento do loop. Depois permite overdubs, mudança de tempo e
reprodução enquanto o músico altera o timbre. Atualizações dos Reface também
oferecem quantização em semicolcheias ou tercinas de colcheia.

A frase comporta até 2.000 notas ou aproximadamente dez minutos a 120 BPM, mas é
perdida ao desligar o aparelho. Isso explicita dois conceitos: comprimento
descoberto a partir da performance e **volatile sketchpad**, no qual a ausência
de persistência favorece captura rápida em vez de edição e arquivo.

### Moog DFAM: lanes de controle analógico

Cada um dos oito steps do DFAM contém valores de pitch e velocity. Esses valores
são tensões de controle: pitch pode modular os osciladores, enquanto velocity
afeta os envelopes de oscilador, filtro e amplitude. Ambos também aparecem como
saídas no patchbay.

O step não precisa representar um objeto Note. O sequenciador produz lanes de
valores; routing e patching determinam seus significados:

```text
step clock -> pitch CV lane ----> destino escolhido
           -> velocity CV lane -> destino escolhido
```

Velocity em zero pode produzir silêncio sem remover a posição temporal. O efeito
se parece com rest, mas emerge do valor de controle em vez de um tipo especial
de evento.

### Moog Subharmonicon: clock separado do valor

O Subharmonicon combina dois sequenciadores de quatro steps com quatro rhythm
generators. Cada sequenciador só avança ao receber pulsos e pode ser dirigido por
um ou vários geradores. Suas saídas também podem ser redirecionadas no patchbay.

```text
ValueSequence --current value--> destino
RhythmGenerator -----pulse-----> advance sequence
```

Essa separação faz polirritmia surgir da relação entre geradores e sequências,
sem exigir micro-timing individual em cada step. É uma referência forte para não
acoplar armazenamento de valores, produção de pulsos e transporte numa única
classe universal.

### Akai APC40 mkII: projeção remota

O APC40 mkII não armazena a sequência. Sua matriz RGB 8x5 apresenta uma janela
sobre clips mantidos pelo Ableton Live, envia comandos de lançamento e recebe de
volta cores e estados.

```text
estado no host <-> viewport físico <-> gesto do performer
```

Clip launch dispara uma unidade existente; scene launch dispara uma combinação
vertical. A superfície pode navegar uma matriz maior que o hardware e continuar
sem ser proprietária dos dados. Isso é prior art direto para uma futura tela M5
que observe e comande estado mantido por outro módulo.

### Arturia KeyStep e KeyLab mk3

O KeyStep representa um sequenciador MIDI/CV tradicional: sequências armazenadas,
rest, tie, gate percentual, swing e divisões temporais, com configurações
independentes para o arpeggiador. Ele confirma que tie não é apenas gate longo:
rest ocupa tempo sem gate, uma nota rearticula e tie prolonga a articulação
anterior pelo próximo step.

O KeyLab mk3, por outro lado, é principalmente um controlador. Arpeggiator,
chord mode e scale mode transformam a performance antes de emitir MIDI; DAW
integration e clip launching comandam estado externo. Seu valor para o radar é
o pipeline combinável de transformações e a separação entre porta MIDI musical
e porta lógica de controle da DAW, não um sequenciador autônomo inexistente.

### Moog Grandmother e Matriarch

O Grandmother diferencia rest, tie e legato. Tie prolonga a mesma nota; legato
muda a nota preservando uma transição articulada sem o comportamento de um novo
ataque convencional. Sua memória contém três sequências de até 256 notas.

O Matriarch amplia o conteúdo do step: cada posição pode conter até quatro notas
e ainda incluir tie ou ratchets. Ele expõe CV, velocity e gate separadamente no
patchbay. A cardinalidade de notas passa a ser parte do formato sequenciado, não
apenas uma capacidade posterior do synth.

Essas referências sugerem `Rest`, `Retrigger`, `Tie` e `Legato` como articulações
distintas e um possível conjunto fixo de notas por step. Elas não justificam uma
API compartilhada até existir um sequenciador melódico nosso.

### K.O. II

O K.O. II já possui seção própria neste radar. Nesta seleção ele reforça que uma
boa experiência de synth sequencing pode residir na hierarquia de composição e
no workflow de variações, não apenas na riqueza de cada step.

## Referências verificadas

- [Koala Sampler Manual — Sample tab](https://manual.koalasampler.com/mobile/4-sample/):
  one-shot, gate por duração do toque, loop/hold, envelope e choke.
- [Roland P-6 — Understanding samples](https://static.roland.com/manuals/p-6/en-US/112809355114018187.html):
  configurações por sample, incluindo gate/one-shot, loop e mono/polifonia.
- [Roland P-6 — Recording with the pattern sequencer](https://static.roland.com/manuals/p-6/en-US/112170507114050443.html):
  pitch, velocity, probability, gate time, micro-timing, sub-steps e motion.
- [Roland P-6 — product overview](https://www.roland.com/us/products/p-6/):
  chop, step sampling, resampling, sequenciador de 64 steps, Step Loop e síntese
  granular.
- [Teenage Engineering PO-32 tonic guide](https://teenage.engineering/guides/po-32/en):
  grade de 16 steps, accent, parameter locks, step multiplier, mute/solo,
  punch-in effects e pattern chaining.
- [Teenage Engineering PO-33 K.O! guide](https://teenage.engineering/guides/po-33/en):
  slots melódicos e de bateria, slices, trim, parameter locks e pattern chaining.
- [Teenage Engineering PO-20 arcade guide](https://teenage.engineering/guides/po-20/en):
  chord control e chaining, parameter locks, step multiplier, efeitos e pattern
  chaining.
- [Korg Volca Beats Owner's Manual](https://cdn.korg.com/us/support/download/files/faa5792adfeaf04839e7abbf1c321acf.pdf):
  Active Step, Step Jump, Stutter, motion recording e sync por pulso.
- [Korg Volca Keys Owner's Manual](https://cdn.korg.com/us/support/download/files/93f9f953416733d196f8d817bb3bc668.pdf):
  Flux, gravação quantizada e Step Trigger.
- [Korg Volca Drum Owner's Manual](https://cdn.korg.com/us/support/download/files/ce079f2162680b4a105c85de32c00e6f.pdf):
  choke configurável por parte.
- [Korg Volca Sample overview](https://www.korg.com/us/products/dj/volca_sample/index.php):
  Active Step, Step Jump, swing e motion sequencing.
- [1010music Nanobox Razzmatazz User Guide](https://1010music.com/wp-content/uploads/2022/10/Nanobox-Razzmatazz-User-Guide-v1-1-0.pdf):
  oito pads, dezesseis sequences por preset, step length/count, gravação
  quantizada, velocity, swing, copy/paste/double e prioridade de clock externo.
- [Behringer Crave Quick Start Guide](https://mediadl.musictribe.com/download/documents/behringer/CRAVE/CRAVE_QSG_WW.pdf):
  notes/rests, gate length, accent, ratchet, pages, pattern end, reset, hold,
  arpeggiador e saídas CV/gate.
- [Behringer Crave product overview](https://www.behringer.com/en/products/0718-AAJ):
  sequenciador de 32 steps, 64 patterns em oito bancos e oito ordens de
  arpeggiador.
- [Teenage Engineering EP-133 workflow](https://teenage.engineering/guides/ep-133/workflow):
  projects, groups, patterns, scenes, song positions e commit.
- [Teenage Engineering EP-133 sound modes](https://teenage.engineering/guides/ep-133/modes):
  oneshot, key, legato, trim e envelope.
- [Teenage Engineering EP-133 play and record](https://teenage.engineering/guides/ep-133/play-and-record):
  automação latching, escopo de volume, erase, undo e offsets temporais.
- [Teenage Engineering EP-133 functions](https://teenage.engineering/guides/ep-133/functions):
  quantização seletiva, free time, note repeat e loop window.
- [Teenage Engineering EP-133 effects](https://teenage.engineering/guides/ep-133/effects):
  effects por group e punch-in effects combináveis sensíveis à pressão.
- [Floyd Steinberg — Tiny Sequencers Compared](https://www.youtube.com/watch?v=NyjVV-oM45Q):
  índice comparativo usado para descobrir os dispositivos deste survey.
- [Wavy Industries Monkey manual](https://wavyindustries.com/monkey/manual/):
  loop de 16 beats, overdub checkpoints, undo e mute/erase por canal MIDI.
- [picoTracker manual — introduction](https://manual.xiphonics.com/advance/introduction.html),
  [chains](https://manual.xiphonics.com/advance/chains.html) e
  [tables](https://manual.xiphonics.com/advance/tables.html): tracker de oito
  canais, chains de phrases e sequências reutilizáveis de comandos.
- [RetroKits RK-008 manual](https://retrokits.com/rk008/RK008_manual.pdf):
  gravação MIDI em tempo real, routing por track, modificadores não destrutivos,
  mixdown, merge e explode.
- [MicroDexed Touch project](https://codeberg.org/positionhigh/MicroDexed-touch)
  e [manual](https://www.musikandmore.net/wp-content/uploads/2025/12/MicroDexed-touch-manual.pdf):
  sequenciadores tracker/pattern e live executáveis em paralelo.
- [Woovebox product documentation](https://www.woovebox.com/): conditionals,
  modifiers, probability, polyrhythm, generation, randomization e song mode.
- [Donner MEDO product documentation](https://www.donnermusic.com/products/medo)
  e [manual](https://cdn.accentuate.io/15091455787394/1757424260901/MEDO_maunal.pdf?v=1757424260901):
  looper, modos instrumentais, quantização e controles por gesto.
- [Synth sequencer tier-list video](https://www.youtube.com/watch?v=TruXg9mUsIU):
  índice de descoberta; o ranking não é usado como evidência técnica.
- [Yamaha Reface owner's manual](https://usa.yamaha.com/files/download/other_assets/6/438816/ZT92080_reface_En_OM_C0.pdf):
  Phrase Looper, key-on start, overdub, capacidade e armazenamento volátil.
- [Moog DFAM manual](https://api.moogmusic.com/sites/default/files/2018-04/DFAM_Manual.pdf):
  lanes analógicas de pitch e velocity e respectivas saídas CV.
- [Moog Subharmonicon manual](https://api.moogmusic.com/sites/default/files/2020-05/Subharmonicon_Manual.pdf):
  dois sequenciadores, quatro rhythm generators e routing pelo patchbay.
- [Akai APC40 mkII user guide](https://cdn.inmusicbrands.com/akai/attachments/apc40II/APC40%20mkII%20-%20User%20Guide%20-%20v1.0.pdf):
  viewport 8x5, feedback RGB, clip launch e scene launch no Ableton Live.
- [Arturia KeyStep manual](https://downloads.arturia.net/products/keystep/manual/KeyStep_Manual_1_1_1_EN.pdf):
  sequências, rest, tie, gate, swing, divisões temporais e arpeggiador.
- [Arturia KeyLab mk3 overview](https://www.arturia.com/products/hybrid-synths/keylab-mk3/overview)
  e [FAQ](https://support.arturia.com/hc/en-us/articles/15604245986460-KeyLab-mk3-General-Questions):
  chord, scale, arpeggiator, DAW integration e portas MIDI/DAW separadas.
- [Moog Grandmother manual](https://api.moogmusic.com/sites/default/files/2022-01/Grandmother_Manual_Version_2.pdf):
  sequências, rest, tie e legato.
- [Moog Matriarch manual](https://api.moogmusic.com/sites/default/files/2019-08/Moog-Matriarch-Manual.pdf):
  steps com até quatro notas, tie, ratchets e saídas CV/velocity/gate.

Links e comportamentos foram verificados em 2026-09-25. Uma atualização futura
deve preservar a data e distinguir documentação oficial de inferências nossas.

## Regra de evolução

Adicionar um item ao radar requer uma fonte ou observação reproduzível. Torná-lo
uma prioridade exige um experimento concreto. Um módulo compartilhado pode ser
criado experimentalmente quando a fronteira parecer promissora; seus contratos
devem continuar explícitos e revisáveis conforme os consumidores reais tragam
novas evidências.
