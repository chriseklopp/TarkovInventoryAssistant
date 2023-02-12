namespace TarkovInventoryAssistant_Server.Models
{
    public class DetectionResultsModel
    {

        public DetectionResultsModel()
        {

        }

        public string? RequestId { get; set; }

        public int catalogImage { get; set; }

        public int sourceImage { get; set; }

        public string name { get; set; } = "";

        public string dim { get; set; } = "";

        public int fleaPrice { get; set; }

        public int pricePerSlot { get; set; }

        public int traderPrice { get; set; }

        public string trader { get; set; } = "";

        public int parentImageID { get; set; }

    }
}