package com.game.rabbitmq;

public class MqConfig {
	
	private static MqConfig instance = null;

	private MqConfig() {
	};

	public static MqConfig getInstance() {
		if (instance == null) {
			synchronized (MqConfig.class) {
				if (instance == null) {
					instance = new MqConfig();
				}
			}
		}
		return instance;
	}
	
	private String ip;
	private int port;
	private String user;
	private String password;
	private String queueName;
	private String exchangeName;
	public String getExchangeName() {
		return exchangeName;
	}
	public void setExchangeName(String exchangeName) {
		this.exchangeName = exchangeName;
	}
	public String getIp() {
		return ip;
	}
	public void setIp(String ip) {
		this.ip = ip;
	}
	public int getPort() {
		return port;
	}
	public void setPort(int port) {
		this.port = port;
	}
	public String getUser() {
		return user;
	}
	public void setUser(String user) {
		this.user = user;
	}
	public String getPassword() {
		return password;
	}
	public void setPassword(String password) {
		this.password = password;
	}
	public String getQueueName() {
		return queueName;
	}
	public void setQueueName(String queueName) {
		this.queueName = queueName;
	}
	
}
