require 'socket'
require 'thread'

socket_path = "/tmp/socket_server.sock"

def handle_client(client_socket, client_id)
  while true
    message = client_socket.gets
    break if message.nil?

    message.chomp!

    puts "Cliente #{client_id}: #{message}"
  end
  client_socket.close
end


def server(socket_path)
  if File.exist?(socket_path)
    File.delete(socket_path)
  end


  server_socket = UNIXServer.new(socket_path)
  puts "Servidor aguardando conexões..."

  loop do
    client_socket = server_socket.accept
    
    client_id = client_socket.gets.chomp
    puts "Novo cliente conectado: #{client_id}"

    Thread.new { handle_client(client_socket, client_id) }
  end
end

server(socket_path)
