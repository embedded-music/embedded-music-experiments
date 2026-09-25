# 056 — Nanobox Razzmatazz no radar de semânticas

## Objetivo

Pesquisar o 1010music Nanobox Razzmatazz como uma referência próxima ao nosso
drum sequencer e registrar apenas as semânticas que podem orientar os próximos
experimentos de patterns, transporte e edição.

## Observações

O manual oficial descreve oito pads, dezesseis slots de sequence por preset,
step length separado de step count (1 a 64), gravação quantizada de pads/MIDI
com velocity, edição em uma grade com cursor, copy/paste/clear/double e troca de
sequence ao fim da sequence corrente. Swing é alterável em tempo real. Para
sincronização, a prioridade documentada é Clock In analógico, MIDI Clock e, por
fim, BPM interno.

## Consequências para nossos experimentos

Estas observações reforçam quatro candidatos concretos: uma sequence deve poder
ser material armazenável distinto do transporte; `queue next pattern` deve ser
uma operação quantizada; duração lógica deve poder divergir da grade visível; e
uma fonte/fallback de clock deve ser distinguível do valor BPM. O Razzmatazz é
referência de semântica, não um contrato a ser copiado: touchscreen, SD, FM,
samples e mixer estão fora do escopo atual.

## Fonte e validação

- [Nanobox Razzmatazz User Guide v1.1.0](https://1010music.com/wp-content/uploads/2022/10/Nanobox-Razzmatazz-User-Guide-v1-1-0.pdf), páginas 24–32 e 84.
- Nenhuma alteração de firmware foi feita nesta pesquisa.
