#!/bin/bash

# Последовательный тест всех блоков Mega #2 (9-15)
# Каждый блок: ВВЕРХ 5 сек → пауза 3 сек → ВНИЗ 5 сек → пауза 3 сек
# Блоки работают ПООЧЕРЕДНО (по одному)

PORT="/dev/cu.usbmodem141021"
BAUD="115200"

echo "=========================================="
echo "  ПОСЛЕДОВАТЕЛЬНЫЙ ТЕСТ MEGA #2"
echo "  Блоки 9-15 (7 blocks)"
echo "=========================================="
echo ""

# Функция отправки команды
send_command() {
    local cmd=$1
    echo "[SEND] $cmd"
    echo "$cmd" > "$PORT"
    sleep 0.3
}

# Тест одного блока (UP → пауза → DOWN → пауза)
test_block() {
    local block_num=$1
    local up_duration=$2
    local down_duration=$3
    local pause=$4

    echo ""
    echo "╔═══════════════════════════════════════╗"
    echo "║       БЛОК $block_num                        ║"
    echo "╚═══════════════════════════════════════╝"

    # ВВЕРХ
    echo "  ↑ Поднимаем блок $block_num..."
    send_command "{\"block\":$block_num,\"action\":\"up\",\"duration\":$up_duration}"

    # Ждём окончания подъёма + пауза
    local total_wait=$(echo "scale=1; $up_duration/1000 + $pause" | bc)
    echo "  ⏳ Ожидание ${total_wait}с (подъём + пауза)"
    sleep $total_wait

    # ВНИЗ
    echo "  ↓ Опускаем блок $block_num..."
    send_command "{\"block\":$block_num,\"action\":\"down\",\"duration\":$down_duration}"

    # Ждём окончания спуска + пауза
    total_wait=$(echo "scale=1; $down_duration/1000 + $pause" | bc)
    echo "  ⏳ Ожидание ${total_wait}с (спуск + пауза)"
    sleep $total_wait

    echo "  ✅ Блок $block_num завершён"
}

# Настройка порта
echo "[INIT] Настройка порта $PORT на $BAUD baud..."
stty -f "$PORT" "$BAUD"
sleep 2

echo "[INIT] Готов к тестированию!"
echo ""
echo "Начинаю последовательный тест через 2 секунды..."
sleep 2

# ============================================
# ОСНОВНОЙ ЦИКЛ: ТЕСТ БЛОКОВ 9-15 ПООЧЕРЕДНО
# ============================================

# Параметры теста
UP_DURATION=10000     # 10 секунд вверх
DOWN_DURATION=10000   # 10 секунд вниз
PAUSE_BETWEEN=3       # 3 секунды пауза между действиями

echo ""
echo "=========================================="
echo "  ПАРАМЕТРЫ ТЕСТА:"
echo "  - Подъём: ${UP_DURATION}мс"
echo "  - Спуск: ${DOWN_DURATION}мс"
echo "  - Пауза: ${PAUSE_BETWEEN}с"
echo "=========================================="

# Последовательно тестируем каждый блок
for block in {9..15}; do
    test_block $block $UP_DURATION $DOWN_DURATION $PAUSE_BETWEEN
done

echo ""
echo "=========================================="
echo "  ✅ ВСЕ БЛОКИ ПРОТЕСТИРОВАНЫ!"
echo "  Всего блоков: 7 (9-15)"
echo "=========================================="
echo ""
echo "Проверь:"
echo "  [ ] Все блоки поднялись"
echo "  [ ] Все блоки опустились"
echo "  [ ] Не было одновременной работы"
echo "  [ ] Блок 15 (3 актуатора) работает корректно"
echo ""
