def imprimir_tabela():
    tabela = [
        ["", "if", "then", "a", "b", "$", "S", "E", "C"],
        ["0", "s4", "", "", "s3", "", "1", "", "2"],
        ["1", "", "", "", "", "acc", "", "", ""],
        ["2", "r2", "r2", "r2", "r2", "r2", "", "", ""],
        ["3", "r4", "r4", "r4","r4","r4","","",""],
        ["4","","","","s5","","","6",""],
        ["5","r3","r3","r3","r3","r3","","",""],
        ["6","","s7","","","","","",""],
        ["7","","","","s3","","","","8"],
        ["8","r1","r1","r1","r1","r1","","",""]
    ]

    for linha in tabela:
        print("| {:<5} | {:<5} | {:<5} | {:<5} | {:<5} | {:<5} | {:<5} | {:<5} | {:<5} |".format(*linha))
        print("-" * 73)

def dividir_em_tokens(entrada):
    return entrada.split()

def analisar(entrada):
    pilha = []
    tokens = dividir_em_tokens(entrada)
    posicao_atual = 0

    # Empilhar símbolo inicial da pilha
    pilha.append("$")
    pilha.append("S")

    print(f"Analisando a entrada '{entrada}':")

    # Imprimir cabeçalho da tabela
    print("| {:<20} | {:<20} | {:<20} |".format("Pilha","Cadeia","Regra"))

    # Analisar entrada
    for simbolo in tokens:
        while True:
            topo_pilha = pilha[-1]
            if topo_pilha == simbolo:
                pilha.pop()
                print("| {:<20} | {:<20} | {:<20} |".format("".join(reversed(pilha)), "".join(tokens[posicao_atual+1:]), ""))
                posicao_atual += 1
                break
            elif topo_pilha == 'S':
                if simbolo == 'if':
                    pilha.pop()
                    pilha.append('C')
                    pilha.append('then')
                    pilha.append('E')
                    pilha.append('if')
                    print("| {:<20} | {:<20} | {:<20} |".format("".join(reversed(pilha)), "".join(tokens[posicao_atual:]),"S -> if E then C"))
                elif simbolo == 'b':
                    pilha.pop()
                    pilha.append('C')
                    print("| {:<20} | {:<20} | {:<20} |".format("".join(reversed(pilha)), "".join(tokens[posicao_atual:]),"S -> C"))
                else:
                    tratar_erro(posicao_atual, tokens)
            elif topo_pilha == 'E':
                if simbolo == 'a':
                    pilha.pop()
                    print("| {:<20} | {:<20} | {:<20} |".format("".join(reversed(pilha)), "".join(tokens[posicao_atual:]),"E -> a"))
                else:
                    tratar_erro(posicao_atual, tokens)
            elif topo_pilha == 'C':
                if simbolo == 'b':
                    pilha.pop()
                    print("| {:<20} | {:<20} | {:<20} |".format("".join(reversed(pilha)), "".join(tokens[posicao_atual:]),"C -> b"))
                else:
                    tratar_erro(posicao_atual, tokens)
            else:
                break

    # Verificar se a pilha está vazia
    if len(pilha) == 1 and pilha[0] == '$':
        print("\nAnálise bem-sucedida")
    else:
        tratar_erro(posicao_atual, tokens)

def tratar_erro(posicao_atual, tokens):
    print(f"Erro: Regra inválida na posição {posicao_atual}, próximo token: '{tokens[posicao_atual]}'")
    exit(1)

imprimir_tabela()
entrada = input("Digite a entrada: ")
analisar(entrada)