CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./headers

SRC_FILES = utf-8_algorithm_test.c unit_test.c

EXECUTABLE = utf8_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(SRC_FILES)
	$(CC) $(CFLAGS) -o $@ $^

test: $(EXECUTABLE)
	@echo "Running tests..."
	@echo "Teste de caracteres ASCII:"
	./$(EXECUTABLE) abcdefghijklmnopqrstuvwxyz
	@echo "Teste de caracteres UTF-8 estendidos:"
	./$(EXECUTABLE) áéíóúüñ
	@echo "Teste de caracteres multibyte:"
	./$(EXECUTABLE) こんにちは
	@echo "Teste de caracteres multibyte estendidos:"
	./$(EXECUTABLE) 😊🌎🚀
	@echo "Teste de caracteres mistos:"
	./$(EXECUTABLE) Hello,こんにちは,😊
	@echo "Teste de caracteres ASCII repetidos:"
	./$(EXECUTABLE) AAAAA
	@echo "Teste de caracteres multibyte repetidos:"
	./$(EXECUTABLE) あああああ
	@echo "Teste de caracteres aleatórios:"
	./$(EXECUTABLE) abcde123\!\@\#\$\%\^\&\*\(\)
	@echo "Teste de caracteres multibyte aleatórios:"
	./$(EXECUTABLE) 🍎🍇🍉🍊🍋
	@echo "Teste de caracteres especiais:"
	./$(EXECUTABLE) ©®™
	@echo "Teste de caracteres de controle:"
	./$(EXECUTABLE) \n\t\r
	@echo "Teste de caracteres misturados:"
	./$(EXECUTABLE) Hello123🌟
	@echo "Teste de caracteres aleatórios em maiúsculas e minúsculas:"
	./$(EXECUTABLE) AbCdEfGhIjK
	@echo "Teste de caracteres multibyte misturados:"
	./$(EXECUTABLE) あ🌟🚗
	@echo "Teste de caracteres ASCII misturados com multibyte:"
	./$(EXECUTABLE) Helloこんにちは
	@echo "Teste de caracteres multibyte misturados com ASCII:"
	./$(EXECUTABLE) 🌟🚗Hello
	@echo "Teste de caracteres com sinais de pontuação:"
	./$(EXECUTABLE) \.\,\;\:\!\?\-
	@echo "Teste de caracteres com números:"
	./$(EXECUTABLE) 1234567890
	@echo "Teste de caracteres com parênteses:"
	./$(EXECUTABLE) \(\)\{\}\[\]
	@echo "Teste de caracteres com aspas:"
	./$(EXECUTABLE) \"\'\`
	@echo "TTeste de caracteres com barras:"
	./$(EXECUTABLE) "/|\\"
	@echo "Teste de caracteres com sinais matemáticos:"
	./$(EXECUTABLE) +-*/=
	@echo "Teste de caracteres com símbolos monetários:"
	./$(EXECUTABLE) \$\€\£\¥
	@echo "Teste de caracteres com símbolos de setas:"
	./$(EXECUTABLE) ←↑→↓
	@echo "Teste de caracteres com símbolos de controle:"
	./$(EXECUTABLE) ⇧⌘⌥⌫
	@echo "Teste de caracteres com símbolos de música:"
	./$(EXECUTABLE) ♩♫♪♬
	@echo "Teste de caracteres com símbolos de jogos:"
	./$(EXECUTABLE) ⚔️🎮🕹️
	@echo "Teste de caracteres com emojis sorridentes:"
	./$(EXECUTABLE) 😀😃😄😁😆
	@echo "Teste de caracteres com emojis tristes:"
	./$(EXECUTABLE) 😢😭😔😞😟
	@echo "Teste de caracteres com emojis de animais:"
	./$(EXECUTABLE) 🐶🐱🐰🐼🐵
	@echo "Teste de caracteres com emojis de alimentos:"
	./$(EXECUTABLE) 🍔🍕🌮🍣🍰
	@echo "Teste de caracteres com emojis de climas:"
	./$(EXECUTABLE) 🌞🌧️❄️🌈🌪️
	@echo "Teste de caracteres com emojis de transporte:"
	./$(EXECUTABLE) 🚗🚲🚂🛳️🚀
	@echo "Teste de caracteres com emojis de plantas:"
	./$(EXECUTABLE) 🌱🌷🌲🌻🌴
	@echo "Teste de caracteres com emojis de objetos:"
	./$(EXECUTABLE) ⌚🖥️📱🔧🛠️
	@echo "Teste de caracteres com emojis de bandeiras:"
	./$(EXECUTABLE) 🇺🇸🇯🇵🇨🇦🇬🇧🇧🇷
	@echo "Teste de caracteres com emojis de corações:"
	./$(EXECUTABLE) ❤️💛💚💙💜
	@echo "Teste de caracteres com emojis de estrelas:"
	./$(EXECUTABLE) ⭐🌟✨💫🌠
	@echo "Teste de caracteres com emojis de lua:"
	./$(EXECUTABLE) 🌙🌕🌖🌗🌘
	@echo "Teste de caracteres com emojis de sol:"
	./$(EXECUTABLE) 🌞🌝🌟☀️🔆
	@echo "Teste de caracteres com emojis de transporte público:"
	./$(EXECUTABLE) 🚆🚈🚄🚇🚝
	@echo "Teste de caracteres com emojis de relógios:"
	./$(EXECUTABLE) 🕛🕧🕐🕜🕑🕒🕓🕔🕕🕖
	@echo "Teste de caracteres com emojis de sinais:"
	./$(EXECUTABLE) ⛔⚠️🚸🚦🚥
	@echo "Teste de caracteres com emojis de gestos:"
	./$(EXECUTABLE) 👋🤝✍️🤲👐
	@echo "Teste de caracteres com emojis de bebidas:"
	./$(EXECUTABLE) 🍺🍻🍷🍹🍾
	@echo "Teste de caracteres com emojis de esportes:"
	./$(EXECUTABLE) ⚽🏀🏈⚾🎾

	@echo "Caracteres inválidos"
	@echo "Teste 1:"
	-./$(EXECUTABLE) 255
    # "\x80" 
    # "\xC3"        
    # "\xF0\x80\x80"   
    # "\xFE"                  
    # "\xE0"                      
    # "\x20\xC2\x80"              
    # "\x20\xC2\x20"                
    # "\xC2\xC2"                   
    # "\xC2\x80\x20"              
    # "\xE0\xE0\xE0"              
    # "\xE0\x80"                
    # "\xE0\x80\xE0"             
    # "\xE0\x80\x80"              
    # "\xE0\x80\xE0\x80"           
    # "\xE0\xE0\xE0\x80"          
    # "\xE0\x80\x80\x80"          
    # "\xF0\xF0\xF0"               
    # "\xF0\xF0\x80"               
    # "\xF0\x80\x80\x80"         
    # "\xF0\x80\x80\xF0"        
    # "\xF0\x80\x80\x80\x80"    
    # "\xF0\xF0\xF0\xF0"      
    # "\xF0\xF0\xF0\x80"       
    # "\xF0\xF0\xF0\xF0\xF0"      
    # "\xF0\xF0\x80\xF0"           
    # "\xF0\x80\xF0\xF0"  
    # "\xF0\x80\xF0\xF0\x80"  
    # "\xF0\xF0\xF0\xF0\xF0\x80" 

clean:
	rm -f $(EXECUTABLE)