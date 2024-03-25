
class StatLineInfo:
    def __init__(self, id, value, description) -> None:
        self.id = id;
        self.value = value;
        self.description = description

    def __init__(self, id, value) -> None:
        self.id = id;
        self.value = value

    def __str__(self):
        return f"ID: {self.id} / Value: {self.value} / Description: {self.description}"

def read_file(file_name):
    try:
        file_info = []
        with open(file_name, "r") as file:
            for line in file:
                content = line.strip().split(' ', maxsplit=9000);
                if(len(content) == 3):
                    id, value, description = content;
                    file_info.append(StatLineInfo(id, value, description))

                if(len(content) == 2):
                    id, value, description = content;
                    file_info.append(StatLineInfo(id, value))

        return file_info

    except FileNotFoundError:
        print(f"Arquivo {file_name} não encontrado.")


# Exemplo de uso
nome_do_arquivo = "E:\GitHub Projects\evaluation-architecture-computers\stats-script\stats.txt"

file_content = read_file(nome_do_arquivo)
for value in file_content:
    print(value)

