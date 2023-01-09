<?
class MYSQL 
{
	var $_Host;
	var $_User;
	var $_Pass;
	var $_DBName;
	var $_dbResult;
	var $_dbConID;	
	
	function MYSQL() 	{
		$this->_Host = "";
		$this->_User = "";
		$this->_Pass = "";
		$this->_DBName = "";
	}
	
	function Connect(){
		$this->_dbConID = mysql_connect($this->_Host, $this->_User, $this->_Pass) or die("Problème lors de la connexion");
		if ($this->_dbConID) mysql_select_db($this->_DBName, $this->_dbConID);
	}
	
	function Execute($query) {
		if ($this->_dbConID) $this->_dbResult = mysql_query($query, $this->_dbConID) or die("Problème lors de la requête SQL");
	}
	
	function NumRows() {
		if ($this->_dbResult) {
			return mysql_num_rows($this->_dbResult);
		} else {
			return false;
		}
	}
	
	function FetchArray() {
		if ($this->_dbConID) return mysql_fetch_array($this->_dbResult);
	}
	
	function QueryResult($row) {
		if ($this->_dbConID) return mysql_result($this->_dbResult, $row);
	}
	
	function ClearResults() {
		if ($this->_dbResult) {
			mysql_free_result($this->_dbResult);
			$this->_dbResult='';
		}
	}
	
	function LastInsertID() {
		if ($this->_dbConID) return mysql_insert_id($this->_dbConID);
	}
	
	function Disconnect() {
		if ($this->_dbConID) mysql_close($this->_dbConID);
	}
	
	function Query($sql)
	{
		$result = mysql_query($sql) or die('Erreur SQL !<br />'.$sql.'<br />' . mysql_error());	
		
		$rows = array();
		
		if (gettype($result) != 'boolean')
		{
			while ($data = mysql_fetch_array($result))
			{
				array_push($rows, $data);
			}
			
			mysql_free_result($result);
			
			return $rows;
		}
		else
		{
			return false;
		}
	}
}
?>