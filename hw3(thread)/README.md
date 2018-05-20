# Server

分成 ClientHandler 和 Server 兩個 Class

## Class Server

* 接收 Client 的連線請求 (listen & accept)
* 新建一個 ClientHandler 並將它丟給 Thread 執行

## Class ClientHandler

* 負責所有 Server-Client 間的溝通
* 使用 Map 儲存所有的 Client (clientSocketMap)
* 使用 Map 判斷 Client 是否在線 (clientSocketOnline)
* 使用 Map 和 Vector 儲存離線訊息 (clientSocketOffLineMessag)

# Client

* 溝通方式
    * message (broadcast)
    * chat name message (unicast)
    * bye (disconnect)

## Class Client

* 跟 Server 請求連線

## Client Main

* 負責處理鍵盤的輸入
* 利用 Thread 來顯示收到的的訊息