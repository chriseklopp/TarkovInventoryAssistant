using TarkovInventoryAssistant_Server.Models;
using TarkovInventoryAssistant_Server.Utilities;

namespace TarkovInventoryAssistant_Server.source.Models
{
    // Contains information necessary for populating the Result view.
    public class ResultsListModel
    {

        public ResultsListModel() {

            results = new List<DetectionResultsModel>();
            totalvaluecounter = new TotalValueCounter();
        }


        public List<DetectionResultsModel> results { get; set; }
        public TotalValueCounter totalvaluecounter { get; set; }
    }
}
