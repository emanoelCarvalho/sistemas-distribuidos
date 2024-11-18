require 'socket'
require 'time'

socket_path = "/tmp/socket_server.sock"

def send_message(client_socket)
  while true
    print "Digite uma mensagem: "
    message = gets.chomp
    
    break if message.downcase == "sair"
    
    client_socket.puts(message)
  end
end

def client(socket_path)
  client_socket = UNIXSocket.new(socket_path)

  client_id = "Cliente-#{Time.now.to_i}"

  client_socket.puts(client_id)
  puts "Conectado ao servidor com o identificador: #{client_id}"

  send_message(client_socket)

  client_socket.close
end

client(socket_path)
