require 'socket'
require 'time'

# Caminho do socket local
socket_path = "/tmp/socket_server.sock"

# Função para enviar mensagens ao servidor
def send_message(client_socket)
  while true
    # Aguarda o input do usuário
    print "Digite uma mensagem: "
    message = gets.chomp
    
    # Se o usuário digitar "sair", encerra a conexão
    break if message.downcase == "sair"
    
    # Envia a mensagem ao servidor
    client_socket.puts(message)
  end
end

# Função principal do cliente
def client(socket_path)
  # Cria o socket e conecta ao servidor
  client_socket = UNIXSocket.new(socket_path)

  # Gera um identificador único
  client_id = "Cliente-#{Time.now.to_i}"

  # Envia o identificador único ao servidor
  client_socket.puts(client_id)
  puts "Conectado ao servidor com o identificador: #{client_id}"

  # Envia mensagens do usuário
  send_message(client_socket)

  # Fecha a conexão ao final
  client_socket.close
end

# Inicia o cliente
client(socket_path)
