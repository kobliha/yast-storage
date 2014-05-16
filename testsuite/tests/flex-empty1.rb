# encoding: utf-8

# testedfiles: helper.rb

module Yast

  class TestClient < Client

    def main

      def setup1()
        setup_system("empty")

        setup_part_info(<<-EOT)
PARTITION  mount=/      size=8GiB
PARTITION  mount=swap   size=1GB
PARTITION  mount=/data  sizepct=100
EOT
      end

      def setup2()
        ProductFeatures.SetBooleanFeature("partitioning", "try_separate_home", false)
        ProductFeatures.SetBooleanFeature("partitioning", "proposal_lvm", false)
        ProductFeatures.SetBooleanFeature("partitioning", "proposal_snapshots", false)
        ProductFeatures.SetStringFeature("partitioning", "vm_desired_size", "30 GB")
        ProductFeatures.SetStringFeature("partitioning", "root_base_size", "20 GB")
      end

      def setup3()
      end

      Yast.include self, "helper.rb"

    end

  end

end

Yast::TestClient.new.main
