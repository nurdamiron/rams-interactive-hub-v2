#!/bin/bash

# Тестовый скрипт для проверки всех блоков Mega #2 (блоки 9-15)
# Каждый блок: ВВЕРХ 5 сек → пауза 2 сек → ВНИЗ 5 сек → пауза 2 сек

PORT="/dev/cu.usbmodem141021"
BAUD="115200"

echo "=========================================="
echo "  ТЕСТ ВСЕХ БЛОКОВ MEGA #2 (9-15)"
echo "=========================================="
echo ""

# Функция отправки команды
send_command() {
    local cmd=$1
    echo "[SEND] $cmd"
    echo "$cmd" > "$PORT"
    sleep 0.5
}

# Тест одного блока (UP → пауза → DOWN → пауза)
test_block() {
    local block_num=$1
    local duration=$2
    local pause=$3

    echo ""
    echo "=== БЛОК $block_num ==="

    # ВВЕРХ
    echo "→ Блок $block_num ВВЕРХ ($duration мс)"
    send_command "{\"block\":$block_num,\"action\":\"up\",\"duration\":$duration}"
    sleep $(echo "scale=1; $duration/1000 + $pause" | bc)

    # ВНИЗ
    echo "→ Блок $block_num ВНИЗ ($duration мс)"
    send_command "{\"block\":$block_num,\"action\":\"down\",\"duration\":$duration}"
    sleep $(echo "scale=1; $duration/1000 + $pause" | bc)

    echo "✓ Блок $block_num завершён"
}

# Настройка порта
stty -f "$PORT" "$BAUD"
sleep 2

echo "Запуск теста через 2 секунды..."
sleep 2

# ТЕСТ ВСЕХ БЛОКОВ (9-15)
for block in {9..15}; do
    test_block $block 5000 2
done

echo ""
echo "=========================================="
echo "  ✅ ТЕСТ ЗАВЕРШЁН!"
echo "  Протестировано блоков: 7 (9-15)"
echo "=========================================="
